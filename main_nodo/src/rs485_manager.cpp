/**
 * @file rs485_manager.cpp
 * @brief Implementación del gestor de comunicación RS485 para módulo sensor externo.
 *
 * Esta implementación proporciona la funcionalidad completa para comunicarse
 * con un módulo sensor externo a través de RS485, incluyendo control de
 * dirección, validación de checksum y manejo de errores.
 *
 * @author Tu Nombre
 * @date 2025
 * @version 1.0
 */

#include "rs485_manager.h"

// Códigos de error
#define RS485_ERROR_NONE 0
#define RS485_ERROR_TIMEOUT -1
#define RS485_ERROR_CHECKSUM -2
#define RS485_ERROR_COMMUNICATION -3
#define RS485_ERROR_INVALID_DATA -4

RS485Manager::RS485Manager(SoftwareSerial* serial, 
                           uint8_t controlPin,
                           uint32_t baudrate,
                           uint32_t timeout,
                           uint8_t retryCount) 
    : serial(serial), controlPin(controlPin), baudrate(baudrate), 
      timeout(timeout), retryCount(retryCount), lastError(RS485_ERROR_NONE), 
      failedAttempts(0) {
    DEBUG_PRINTF("RS485Manager: Constructor inicializado\n");
}

RS485Manager::~RS485Manager() {
    DEBUG_PRINTF("RS485Manager: Destructor llamado\n");
}

bool RS485Manager::begin() {
    if (!serial) {
        DEBUG_PRINTF("RS485Manager: Error - Serial no inicializado\n");
        lastError = RS485_ERROR_COMMUNICATION;
        return false;
    }
    
    // Configurar pin de control
    pinMode(controlPin, OUTPUT);
    
    // Inicializar en modo lectura (LOW)
    digitalWrite(controlPin, LOW);
    
    // Inicializar comunicación serial (SoftwareSerial)
    serial->begin(baudrate, SWSERIAL_8N1, RS485_RX_SOFTWARE, RS485_TX_SOFTWARE, false, 256);
    
    DEBUG_PRINTF("RS485Manager: Inicializado en baudrate %d\n", baudrate);
    return true;
}

bool RS485Manager::requestSensorData(Protocol::GroundSensor& sensorData) {
    for (uint8_t attempt = 0; attempt <= retryCount; attempt++) {
        if (attempt > 0) {
            delay(100); // Pequeña pausa entre reintentos
        }

        // Limpiar buffer antes de transmitir
        while (serial->available()) serial->read();

        // Enviar solicitud binaria: 0x20
        digitalWrite(controlPin, HIGH); // TX
        delay(RS485_TX_DELAY);
        uint8_t req = (uint8_t)Protocol::RS485Type::REQ_GROUND_DATA;
        serial->write(req);
        serial->flush();
        digitalWrite(controlPin, LOW); // RX
        delay(12); // respiro antes de leer

        // Esperar tipo de respuesta
        unsigned long startTime = millis();
        while ((millis() - startTime) < timeout && serial->available() < 1) {
            yield();
        }
        if (serial->available() < 1) {
            DEBUG_PRINTF("RS485Manager: Timeout esperando tipo de respuesta (%lu ms)\n", timeout);
            lastError = RS485_ERROR_TIMEOUT;
            continue; // reintentar
        }

        uint8_t respType = (uint8_t)serial->read();
        DEBUG_PRINTF("RS485Manager: Tipo respuesta=0x%02X\n", respType);
        if (respType != (uint8_t)Protocol::RS485Type::DATA_GROUND) {
            DEBUG_PRINTF("RS485Manager: Tipo inesperado (esperado 0x30)\n");
            lastError = RS485_ERROR_INVALID_DATA;
            continue; // reintentar
        }

        // Leer payload binario exacto (13 bytes)
        Protocol::GroundSensorBinary payload;
        size_t expected = sizeof(Protocol::GroundSensorBinary);
        startTime = millis();
        while ((millis() - startTime) < timeout && (size_t)serial->available() < expected) {
            yield();
        }
        if ((size_t)serial->available() < expected) {
            DEBUG_PRINTF("RS485Manager: Timeout esperando %d bytes de payload\n", (int)expected);
            lastError = RS485_ERROR_TIMEOUT;
            continue; // reintentar
        }

        size_t got = serial->readBytes((char*)&payload, expected);
        DEBUG_PRINTF("RS485Manager: Payload bytes leidos=%d (esperado=%d)\n", (int)got, (int)expected);
        if (got != expected) {
            lastError = RS485_ERROR_COMMUNICATION;
            continue;
        }

        // Mapear a estructura interna
        sensorData.temp = payload.temp;
        sensorData.moisture = payload.moisture;
        sensorData.n = payload.n;
        sensorData.p = payload.p;
        sensorData.k = payload.k;
        sensorData.EC = payload.EC;
        sensorData.PH = payload.PH;

        lastError = RS485_ERROR_NONE;
        failedAttempts = 0;
        return true;
    }

    failedAttempts++;
    return false;
}

// Métodos simplificados eliminados - ya no necesarios con el protocolo de texto

bool RS485Manager::isAvailable() {
    return (serial != nullptr && failedAttempts < 5);
}

int RS485Manager::getLastError() const {
    return lastError;
}

uint8_t RS485Manager::getFailedAttempts() const {
    return failedAttempts;
}

bool RS485Manager::sendPing() {
    // Limpiar buffer antes de transmitir
    while (serial->available()) serial->read();
    
    // Enviar PING
    digitalWrite(controlPin, HIGH); // Modo transmisión
    delay(RS485_TX_DELAY);
    serial->write((uint8_t)Protocol::RS485Type::PING);
    serial->flush();
    digitalWrite(controlPin, LOW); // Modo recepción
    delay(12); // pequeño respiro antes de leer
    
    // Esperar PONG
    unsigned long startTime = millis();
    while ((millis() - startTime) < timeout && serial->available() < 1) {
        yield();
    }
    
    if (serial->available() >= 1) {
        uint8_t resp = serial->read();
        DEBUG_PRINTF("RS485Manager: PING respuesta=0x%02X\n", resp);
        return (resp == (uint8_t)Protocol::RS485Type::PONG);
    }
    
    DEBUG_PRINTF("RS485Manager: PING timeout\n");
    return false;
}
 