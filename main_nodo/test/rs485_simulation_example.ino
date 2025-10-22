/**
 * @file rs485_simulation_example.ino
 * @brief Ejemplo de uso de la funcionalidad RS485 para simulación de sensores.
 *
 * Este ejemplo demuestra cómo usar la clase RS485Manager y SensorManager
 * para comunicarse con un módulo sensor externo a través de RS485.
 *
 * @author Tu Nombre
 * @date 2025
 * @version 1.0
 */

#include "sensor_manager.h"

// Instancia del gestor de sensores
SensorManager sensorManager;

// Variables para control de tiempo
unsigned long lastSensorRead = 0;
const unsigned long SENSOR_READ_INTERVAL = 5000; // 5 segundos

void setup() {
    Serial.begin(115200);
    Serial.println("=== Ejemplo RS485 Simulación de Sensores (SoftwareSerial) ===");
    
    // Inicializar el gestor de sensores
    sensorManager.begin();
    
    // Habilitar modo simulación RS485
    sensorManager.setSimulationMode(true);
    Serial.println("Modo simulación RS485 habilitado");
    Serial.println("RS485 usando SoftwareSerial (pines 14,15)");
    
    Serial.println("Sistema inicializado correctamente");
    Serial.println();
}

void loop() {
    unsigned long currentMillis = millis();
    
    // Leer sensores cada 5 segundos
    if (currentMillis - lastSensorRead >= SENSOR_READ_INTERVAL) {
        lastSensorRead = currentMillis;
        
        Serial.println("=== Lectura de Sensores ===");
        
        // Leer datos de suelo (incluye RS485 si está habilitado)
        sensorManager.readGroundGpsSensors();
        
        // Mostrar información de debug
        Serial.println(sensorManager.getVoltageReaderDebugInfo());
        Serial.println(sensorManager.getRS485DebugInfo());
        
        // Mostrar datos de suelo
        Serial.println("Datos de suelo:");
        Serial.printf("  Temperatura: %d (décimas de °C)\n", sensorManager.groundData.temp);
        Serial.printf("  Humedad: %d (décimas de %%)\n", sensorManager.groundData.moisture);
        Serial.printf("  pH: %d (décimas)\n", sensorManager.groundData.PH);
        Serial.printf("  EC: %d (μS/cm)\n", sensorManager.groundData.EC);
        Serial.printf("  N: %d (mg/kg)\n", sensorManager.groundData.n);
        Serial.printf("  P: %d (mg/kg)\n", sensorManager.groundData.p);
        Serial.printf("  K: %d (mg/kg)\n", sensorManager.groundData.k);
        
        Serial.println();
        
        // Mostrar datos energéticos
        Serial.println("Datos energéticos:");
        Serial.printf("  Voltaje: %.2f V\n", sensorManager.energyData.volt);
        Serial.printf("  Corriente: %.2f A\n", sensorManager.energyData.amp);
        
        Serial.println();
    }
    
    // Actualizar sensores atmosféricos
    sensorManager.update();
    
    delay(100); // Pequeña pausa para estabilidad
}

/**
 * @brief Función para cambiar entre modo simulación y modo normal.
 * 
 * Esta función puede ser llamada desde el monitor serial para
 * cambiar dinámicamente entre modos de operación.
 */
void toggleSimulationMode() {
    bool currentMode = sensorManager.isSimulationModeEnabled();
    sensorManager.setSimulationMode(!currentMode);
    Serial.printf("Modo simulación cambiado a: %s\n", 
                  sensorManager.isSimulationModeEnabled() ? "HABILITADO" : "DESHABILITADO");
} 