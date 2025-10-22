#pragma once
#include <Arduino.h>

namespace Protocol {
    // Mensajes del protocolo binario (sin framing)
    // Diseño propio, valores hex separados por familias
    // 0x0x: control básico, 0x1x: mantenimiento, 0x2x: solicitudes, 0x3x: datos, 0xEx: errores
    enum RS485Type : uint8_t {
        // Control
        PING              = 0x10, // Ping para verificar presencia
        PONG              = 0x11, // Respuesta a PING

        // Solicitudes
        REQ_GROUND_DATA   = 0x20, // Solicitar datos de suelo (payload: none)

        // Datos
        DATA_GROUND       = 0x30, // Respuesta con GroundSensorBinary (payload: 13 bytes)

        // Mantenimiento/gestión (reservados para futuro)
        NODE_ANNOUNCE     = 0x40, // Anuncio/broadcast de nodo

        // Errores
        ERROR_GENERIC     = 0xE0, // Error genérico (payload opcional)
    };

    // Estructura binaria a enviar (empaquetada, 13 bytes)
    struct __attribute__((packed)) GroundSensorBinary {
        int16_t  temp;      // décimas °C [-400..800]
        uint16_t moisture;  // décimas % [0..1000]
        uint16_t n;         // mg/kg [0..1999]
        uint16_t p;         // mg/kg [0..1999]
        uint16_t k;         // mg/kg [0..1999]
        uint16_t EC;        // uS/cm [0..20000]
        uint8_t  PH;        // décimas [30..90]
    };
    static_assert(sizeof(GroundSensorBinary) == 13, "GroundSensorBinary must be 13 bytes (packed)");
}


