#ifndef CONFIG_H
#define CONFIG_H

// Configuración de pines RS485
#define RS485_DE_PIN 2   // Pin DE/RE del MAX485
#define RS485_RX_PIN 17  // Pin RX del MAX485 (Serial2 RX)
#define RS485_TX_PIN 16  // Pin TX del MAX485 (Serial2 TX)

// Configuración de comunicación
#define BAUD_RATE 9600
#define BUFFER_SIZE 10

// Comandos del protocolo
#define CMD_ILF "ILF"  // Comando para leer datos del sensor

// Formato de respuesta
#define RESPONSE_PREFIX "i"
#define RESPONSE_SUFFIX "f"

// Delays para estabilidad RS485
#define RS485_TX_DELAY 3   // ms (2–5 ms recomendado)
#define RS485_RESPONSE_DELAY 5 // ms

// Rangos de mapeo de potenciómetros
#define POT_MIN 0
#define POT_MAX 4095

// Rangos de salida para cada sensor
#define TEMP_MIN -30.0    // °C
#define TEMP_MAX 50.0     // °C

#define HUMIDITY_MIN 0.0  // %
#define HUMIDITY_MAX 100.0 // %

#define PH_MIN 0.0        // pH
#define PH_MAX 14.0       // pH

#define EC_MIN 0.0        // μS/cm
#define EC_MAX 2000.0     // μS/cm

#define NPK_MIN 0         // ppm
#define NPK_MAX 255       // ppm

#endif 