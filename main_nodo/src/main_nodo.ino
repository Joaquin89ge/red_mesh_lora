
/**
 * @file main_nodo.ino
 * @brief Punto de entrada principal para el nodo sensor de la red mesh agrícola.
 *
 * Este archivo inicializa los módulos clave (identidad, sensores, radio, lógica de aplicación) y gestiona el ciclo de vida del nodo.
 * El flujo principal consiste en:
 *   - Inicialización de la identidad única del nodo (NodeIdentity)
 *   - Inicialización de la comunicación LoRa mesh (RadioManager)
 *   - Inicialización y gestión de sensores (SensorManager)
 *   - Orquestación de la lógica de aplicación (AppLogic)
 *
 * El loop principal mantiene actualizado el nodo, gestionando la adquisición de datos y la comunicación mesh con el gateway central.
 *
 * @see NodeIdentity
 * @see RadioManager
 * @see SensorManager
 * @see AppLogic
 */
// main.cpp
#include <Arduino.h>
#include <WiFi.h>
#include "node_identity.h"
#include "radio_manager.h"
#include "app_logic.h"
#include "sensor_manager.h"
#include "config.h"
#include <esp_task_wdt.h>

// Declaración global de punteros
NodeIdentity* identity = nullptr;
RadioManager* radio = nullptr;
SensorManager* data = nullptr;
AppLogic* logic = nullptr;

// Variables para TWDT
unsigned long lastWatchdogReset = 0;

void setup() {
    Serial.begin(115200);
    delay(600);
    Serial.println("\n--- INICIO DE SETUP PRINCIPAL ---");

    // Configurar Task Watchdog Timer usando defines de config.h
    esp_task_wdt_init(TWDT_TIMEOUT_SECONDS, TWDT_ENABLE_PANIC);
    esp_task_wdt_add(NULL);

    identity = new NodeIdentity();
    identity->begin();

    uint8_t nodeId = identity->getNodeID();

    WiFi.mode(WIFI_OFF); // Evita interferencias de WiFi

    data = new SensorManager();
    data->begin();

    // Inicializa SPI antes de crear RadioManager
    SPI.begin(VSPI_SCK, VSPI_MISO, VSPI_MOSI, VSPI_SS);

    radio = new RadioManager(nodeId);
    if (!radio->init()) {
        Serial.println("Error al inicializar RadioManager");
    }

    logic = new AppLogic(*data, *identity, *radio);
    logic->begin();

    Serial.println("todo ok en nodo");
    
    // Inicializar contador de watchdog
    lastWatchdogReset = millis();
}

void loop() {
    // Reset del TWDT usando intervalo de config.h
    unsigned long currentTime = millis();
    if (currentTime - lastWatchdogReset >= TWDT_RESET_INTERVAL_MS) {
        esp_task_wdt_reset();
        lastWatchdogReset = currentTime;
        Serial.println("[TWDT] Watchdog reseteado");
    }
    
    if (logic) {
        logic->update();
    }
}
