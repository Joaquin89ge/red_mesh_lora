// Protocol.h (protocol bit support)
// 03:00 16/6/2025

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <Arduino.h> // Para tipos como String, uint8_t
#include <vector>    // Para std::vector

/**
 * @file protocol.h
 * @brief Define los tipos de mensajes, estructuras de datos y constantes del protocolo de comunicación mesh LoRa.
 *
 * Este archivo contiene los enums, estructuras y constantes que definen el formato y tipo de los mensajes intercambiados entre nodos y el gateway en la red mesh agrícola.
 * Facilita la interoperabilidad y el parsing de mensajes en la lógica de aplicación.
 *
 * Los rangos de cada variable están documentados en los comentarios de las estructuras.
 * Se utilizan enteros en vez de float para optimizar el uso de memoria y facilitar la transmisión por radio.
 */

namespace Protocol {

    /**
     * @brief Clave de seguridad del protocolo para validación de mensajes.
     */
    const uint8_t KEY = 0x69;

    /**
     * @enum MessageType
     * @brief Tipos de mensajes soportados por el protocolo mesh.
     *
     * Define los códigos de mensaje para ANNOUNCE, HELLO, REQUEST_DATA, DATA, ERROR, etc.
     */
    enum MessageType : uint8_t {
        ANNOUNCE = 0x01,                 /**< Mensaje de anuncio de un nodo. */
        REQUEST_DATA_ATMOSPHERIC = 0x02, /**< Solicitud de datos atmosféricos. */
        REQUEST_DATA_GPC_GROUND = 0x03,  /**< Solicitud de datos gps y ground. */
        DATA_ATMOSPHERIC = 0x04,         /**< Envío de datos atmosféricos. */
        DATA_GPS_CROUND = 0x05,          /**< Envío de datos gps y ground. */
        HELLO = 0x06,                    /**< Mensaje de saludo/conexión inicial. */
        ERROR_DIRECCION = 0x07           /**< Mensaje de dirección de nodo repetida. */
    };

    /**
     * @enum RS485Type
     * @brief Identificadores del protocolo binario RS485 (independiente del protocolo mesh).
     */
    enum RS485Type : uint8_t {
        PING            = 0x10, /**< Ping para verificar presencia del esclavo. */
        PONG            = 0x11, /**< Respuesta a PING. */
        REQ_GROUND_DATA = 0x20, /**< Solicitud de datos de suelo (sin payload). */
        DATA_GROUND     = 0x30, /**< Respuesta con payload GroundSensorBinary (13 bytes). */
        ERROR_GENERIC   = 0xE0  /**< Error genérico (reservado). */
    };

    #pragma pack(push, 1)
    /**
     * @struct AtmosphericSample
     * @brief Estructura para almacenar una muestra de datos atmosféricos.
     *
     * - temp: Temperatura en décimas de grado Celsius [-400 a 800] (-40.0°C a 80.0°C)
     * - moisture: Humedad en décimas de porcentaje [0 a 1000] (0.0% a 100.0%)
     * - hour: Hora de la muestra [0-23]
     * - minute: Minuto de la muestra [0-59]
     */
    struct AtmosphericSample {
        int16_t temp;      ///< Temperatura en décimas de grado [-400 a 800]
        uint16_t moisture; ///< Humedad en décimas de porcentaje [0 a 1000]
        uint8_t hour;      ///< Hora de medición [0-23]
        uint8_t minute;    ///< Minuto de medición [0-59]
    };

    /**
     * @struct GroundSensor
     * @brief Estructura para almacenar datos de sensores de suelo.
     *
     * - temp: Temperatura del suelo en décimas de grado [-400 a 800] (-40.0°C a 80.0°C)
     * - moisture: Humedad del suelo en décimas de porcentaje [0 a 1000] (0.0% a 100.0%)
     * - n, p, k: Nitrógeno, Fósforo, Potasio [0-1999] mg/kg
     * - EC: Conductividad eléctrica [0-20000] μS/cm
     * - PH: pH en décimas [30-90] (3.0 a 9.0)
     */
    struct GroundSensor {
        int16_t temp;      ///< Temperatura en décimas de grado [-400 a 800]
        uint16_t moisture; ///< Humedad en décimas de porcentaje [0 a 1000]
        uint16_t n;        ///< Nitrógeno [0-1999] mg/kg
        uint16_t p;        ///< Fósforo [0-1999] mg/kg
        uint16_t k;        ///< Potasio [0-1999] mg/kg
        uint16_t EC;       ///< Conductividad eléctrica [0-20000] μS/cm
        uint8_t PH;        ///< pH en décimas [30-90] (3.0 a 9.0)
    };

    /**
     * @struct GroundSensorBinary
     * @brief Representación empaquetada (sin padding) para transporte binario por RS485.
     *
     * Layout y tipos alineados con GroundSensor, sin padding. Tamaño esperado: 13 bytes.
     */
    struct __attribute__((packed)) GroundSensorBinary {
        int16_t temp;      ///< décimas °C
        uint16_t moisture; ///< décimas %
        uint16_t n;        ///< mg/kg
        uint16_t p;        ///< mg/kg
        uint16_t k;        ///< mg/kg
        uint16_t EC;       ///< μS/cm
        uint8_t PH;        ///< décimas de pH
    };

    /**
     * @struct GpsSensor
     * @brief Estructura para almacenar datos GPS.
     *
     * - latitude, longitude: Coordenadas en grados * 10^7 [-1800000000 a 1800000000]
     * - altitude: Altitud en metros [-1000 a 9000]
     * - hour, minute: Hora y minuto UTC
     * - flags: Flags de validez de datos (bitmask)
     */
    struct GpsSensor {
        int32_t latitude;   ///< Latitud en grados * 10^7 [-1800000000 a 1800000000]
        int32_t longitude;  ///< Longitud en grados * 10^7 [-1800000000 a 1800000000]
        int16_t altitude;   ///< Altitud en metros [-1000 a 9000]
        uint8_t hour;       ///< Hora GPS [0-23]
        uint8_t minute;     ///< Minuto GPS [0-59]
        uint8_t flags;      ///< Flags de validación (bitmask)
    };

    /**
     * @struct EnergyData
     * @brief Estructura para almacenar datos energéticos del nodo.
     *
     * - volt: Voltaje en centésimas de voltio [0-65535] (0.00V a 655.35V)
     * - amp: Corriente en centésimas de amperio [0-65535] (0.00A a 655.35A)
     * 
     * Los valores se almacenan como enteros sin signo multiplicados por 100
     * para mantener 2 decimales de precisión sin usar float.
     * Ejemplo: 12.34V -> 1234, 5.67A -> 567
     */
    struct EnergyData {
        uint16_t volt; ///< Voltaje en centésimas de voltio
        uint16_t amp;  ///< Corriente en centésimas de amperio
    };

    /**
     * @struct GroundGpsPacket
     * @brief Paquete combinado de datos de suelo y GPS para transmisión.
     */
    struct GroundGpsPacket {
        GroundSensor ground; ///< Datos de suelo
        GpsSensor gps;      ///< Datos GPS
        EnergyData energy;  ///< Datos energéticos
    };
    #pragma pack(pop)

    /**
     * @def MAC_STR_LEN_WITH_NULL
     * @brief Longitud de la cadena MAC (incluyendo null terminator).
     */
    #define MAC_STR_LEN_WITH_NULL 18

} // namespace Protocol

#endif // PROTOCOL_H
