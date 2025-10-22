/**
 * @file rs485_manager.h
 * @brief Gestor de comunicación RS485 para comunicación con módulo sensor externo.
 *
 * Esta clase maneja la comunicación RS485 con un módulo sensor externo que simula
 * lecturas de sensores de suelo (pH, EC, humedad, temperatura, N, P, K).
 * Utiliza el protocolo MAX485 para controlar la dirección de comunicación.
 *
 * Características principales:
 * - Control de pines DE/RE para modo lectura/escritura
 * - Protocolo de comunicación robusto con timeouts
 * - Validación de checksum para integridad de datos
 * - Reintentos automáticos en caso de fallo
 * - Integración con estructuras de datos existentes
 *
 * @author Tu Nombre
 * @date 2025
 * @version 1.0
 */

#ifndef RS485_MANAGER_H
#define RS485_MANAGER_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "protocol.h"
#include "config.h"

/** 
 * @class RS485Manager
 * @brief Clase para gestionar la comunicación RS485 con módulo sensor externo.
 *
 * Esta clase proporciona una interfaz para solicitar y recibir datos de sensores
 * desde un módulo externo a través de comunicación RS485. Maneja automáticamente
 * el control de dirección del transceptor MAX485 y la validación de integridad
 * de los datos recibidos.
 *
 * Ejemplo de uso:
 * @code
 * RS485Manager rs485;
 * rs485.begin();
 * 
 * Protocol::GroundSensor sensorData;
 * if (rs485.requestSensorData(sensorData)) {
 *     // Datos válidos recibidos
 *     Serial.printf("pH: %d, EC: %d\n", sensorData.PH, sensorData.EC);
 * }
 * @endcode
 */
class RS485Manager {
private:
    SoftwareSerial* serial;     ///< Puntero al puerto serial utilizado
    uint8_t controlPin;         ///< Pin para control DE/RE (HIGH=Transmitir, LOW=Recibir)
    uint32_t baudrate;          ///< Velocidad de comunicación
    uint32_t timeout;           ///< Timeout para operaciones
    uint8_t retryCount;         ///< Número de reintentos
    
    // Métodos privados simplificados - protocolo de texto como en el ejemplo de Naylamp

public:
    /**
     * @brief Constructor de la clase RS485Manager.
     * @param serial Puntero al puerto serial a utilizar
     * @param controlPin Pin para control DE/RE (HIGH=Transmitir, LOW=Recibir)
     * @param baudrate Velocidad de comunicación en baudios
     * @param timeout Timeout en milisegundos
     * @param retryCount Número de reintentos
     */
    RS485Manager(SoftwareSerial* serial, 
                 uint8_t controlPin = RS485_RE_DE,
                 uint32_t baudrate = RS485_BAUDRATE,
                 uint32_t timeout = RS485_TIMEOUT_MS,
                 uint8_t retryCount = RS485_RETRY_COUNT);
    
    /**
     * @brief Destructor de la clase RS485Manager.
     */
    ~RS485Manager();
    
    /**
     * @brief Inicializa la comunicación RS485.
     * @return true si la inicialización fue exitosa, false en caso contrario
     */
    bool begin();
    
    /**
     * @brief Solicita datos de sensores al módulo externo.
     * @param sensorData Referencia donde se almacenarán los datos recibidos
     * @return true si se recibieron datos válidos, false en caso contrario
     */
    bool requestSensorData(Protocol::GroundSensor& sensorData);
    
    /**
     * @brief Verifica si la comunicación RS485 está disponible.
     * @return true si está disponible, false en caso contrario
     */
    bool isAvailable();
    
    /**
     * @brief Obtiene el último código de error.
     * @return Código de error de la última operación
     */
    int getLastError() const;
    
    /**
     * @brief Obtiene el número de intentos fallidos consecutivos.
     * @return Número de intentos fallidos
     */
    uint8_t getFailedAttempts() const;
    
    /**
     * @brief Envía un PING para verificar si el esclavo está disponible.
     * @return true si recibe PONG del esclavo, false en caso contrario
     */
    bool sendPing();

private:
    int lastError;              ///< Último código de error
    uint8_t failedAttempts;     ///< Contador de intentos fallidos
};

#endif // RS485_MANAGER_H 