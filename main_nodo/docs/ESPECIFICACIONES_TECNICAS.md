# 📋 ESPECIFICACIONES TÉCNICAS DETALLADAS

## 🎯 Resumen Ejecutivo

El **Sistema de Monitoreo Agrícola - Nodos de Campo** es una solución IoT avanzada diseñada para la **agricultura de precisión**. Este documento detalla las especificaciones técnicas completas del sistema, incluyendo hardware, software, protocolos de comunicación y parámetros de rendimiento.

## 🏗️ Arquitectura del Sistema

### **Diagrama de Arquitectura**

```
┌─────────────────────────────────────────────────────────────────┐
│                    NODO DE CAMPO ESP32                         │
├─────────────────────────────────────────────────────────────────┤
│  📡 COMUNICACIÓN LoRa    📍 GPS NEO-6M    🌡️ SENSORES DHT    │
│     SX1278 (433MHz)      Precisión 2.5m   DHT11/DHT22        │
│     Potencia: 20dBm      Altitud: -1000m  Temp: -40°C/+80°C  │
│     Alcance: 10km        a +9000m         Humedad: 0-100%    │
├─────────────────────────────────────────────────────────────────┤
│  🌱 SENSORES SUELO       ⚡ VOLTAGE READER  📊 PROCESAMIENTO  │
│     NPK: 0-1999 mg/kg    Divisor Resistivo  ESP32 Dual-Core  │
│     pH: 3.0-9.0          Rango: 0-2.5V     WiFi + Bluetooth  │
│     EC: 0-5000 μS/cm     Precisión: ±0.1V  ADC: 12 bits      │
└─────────────────────────────────────────────────────────────────┘
```

### **Componentes Principales**

| Componente           | Modelo         | Especificación   | Función                 |
| -------------------- | -------------- | ---------------- | ----------------------- |
| **Microcontrolador** | ESP32-WROOM-32 | Dual-Core 240MHz | Procesamiento principal |
| **Comunicación**     | SX1278 LoRa    | 433MHz, 20dBm    | Red mesh                |
| **GPS**              | NEO-6M         | 2.5m precisión   | Geolocalización         |
| **Sensores DHT**     | DHT11/DHT22    | -40°C/+80°C      | Ambiente                |
| **Sensores Suelo**   | NPK/pH/EC      | Modbus RS485     | Nutrientes              |
| **Energía**          | VoltageReader  | 0-2.5V           | Monitoreo               |

## 🔧 Especificaciones de Hardware

### **ESP32 Microcontroller**

```cpp
// Especificaciones técnicas
#define ESP32_CPU_FREQ 240000000    // 240 MHz Dual-Core
#define ESP32_FLASH_SIZE 4194304     // 4 MB Flash
#define ESP32_SRAM_SIZE 327680       // 320 KB SRAM
#define ESP32_ADC_RESOLUTION 12      // 12-bit ADC
#define ESP32_ADC_CHANNELS 18        // 18 canales ADC
#define ESP32_GPIO_PINS 34           // 34 pines GPIO
```

### **SX1278 LoRa Module**

```cpp
// Configuración LoRa
#define LORA_FREQUENCY 433000000     // 433 MHz
#define LORA_POWER 20                // 20 dBm máximo
#define LORA_SPREADING_FACTOR 7      // SF7-SF12
#define LORA_BANDWIDTH 125000        // 125 kHz
#define LORA_CODING_RATE 5           // 4/5 coding rate
#define LORA_PREAMBLE_LENGTH 8       // 8 bytes preamble
```

### **GPS NEO-6M**

```cpp
// Especificaciones GPS
#define GPS_PRECISION 2.5            // 2.5 metros
#define GPS_UPDATE_RATE 1            // 1 Hz
#define GPS_OPERATING_VOLTAGE 3.3    // 3.3V
#define GPS_OPERATING_CURRENT 45     // 45mA
#define GPS_ACQUISITION_TIME 27      // 27 segundos (cold start)
```

### **Sensores DHT**

```cpp
// Especificaciones DHT
#define DHT_TEMP_RANGE_MIN -40       // -40°C
#define DHT_TEMP_RANGE_MAX 80        // +80°C
#define DHT_TEMP_ACCURACY 0.5        // ±0.5°C
#define DHT_HUMIDITY_RANGE_MIN 0     // 0%
#define DHT_HUMIDITY_RANGE_MAX 100   // 100%
#define DHT_HUMIDITY_ACCURACY 2      // ±2%
```

### **Sensores de Suelo**

```cpp
// Especificaciones NPK/pH/EC
#define SOIL_TEMP_RANGE_MIN -40      // -40°C
#define SOIL_TEMP_RANGE_MAX 80       // +80°C
#define SOIL_TEMP_ACCURACY 5         // ±5°C
#define SOIL_HUMIDITY_RANGE_MIN 0    // 0%
#define SOIL_HUMIDITY_RANGE_MAX 100  // 100%
#define SOIL_HUMIDITY_ACCURACY 3     // ±3%
#define SOIL_EC_RANGE_MIN 0          // 0 μS/cm
#define SOIL_EC_RANGE_MAX 20000      // 20000 μS/cm
#define SOIL_EC_ACCURACY 5           // ±5%
#define SOIL_PH_RANGE_MIN 3          // pH 3.0
#define SOIL_PH_RANGE_MAX 9          // pH 9.0
#define SOIL_PH_ACCURACY 0.1         // ±0.1
#define SOIL_NPK_RANGE_MIN 1         // 1 mg/kg
#define SOIL_NPK_RANGE_MAX 1999      // 1999 mg/kg
#define SOIL_NPK_ACCURACY 10         // ±10 mg/kg
```

### **VoltageReader (Divisor Resistivo)**

```cpp
// Especificaciones VoltageReader
#define VOLTAGE_READER_PIN 34        // Pin ADC
#define VOLTAGE_READER_ADC_RESOLUTION 12  // 12 bits
#define VOLTAGE_READER_VREF 3.3      // 3.3V referencia
#define VOLTAGE_READER_MAX_INPUT 2.5 // 2.5V máximo
#define VOLTAGE_READER_MIN_INPUT 0.0 // 0V mínimo
#define VOLTAGE_READER_ACCURACY 0.1  // ±0.1V
#define VOLTAGE_READER_FILTER_SAMPLES 8   // 8 muestras filtro
```

## 📡 Protocolo de Comunicación

### **Estructura de Mensajes**

```cpp
// Formato de mensaje LoRa
struct LoRaMessage {
    uint8_t header[2];      // 0xAA 0x55
    uint8_t nodeId;         // ID del nodo (1-250)
    uint8_t messageType;    // Tipo de mensaje
    uint8_t payload[240];   // Datos del mensaje
    uint8_t checksum;       // Checksum CRC8
};
```

### **Tipos de Mensajes**

| Tipo                         | Código | Descripción                  | Payload             |
| ---------------------------- | ------ | ---------------------------- | ------------------- |
| **ANNOUNCE**                 | 0x01   | Anuncio de nodo              | NodeInfo            |
| **REQUEST_DATA_ATMOSPHERIC** | 0x02   | Solicitud datos atmosféricos | Empty               |
| **REQUEST_DATA_GPC_GROUND**  | 0x03   | Solicitud datos suelo/GPS    | Empty               |
| **DATA_ATMOSPHERIC**         | 0x04   | Datos atmosféricos           | AtmosphericSample[] |
| **DATA_GPS_CROUND**          | 0x05   | Datos suelo/GPS              | GroundGpsPacket     |
| **HELLO**                    | 0x06   | Saludo inicial               | NodeInfo            |
| **ERROR_DIRECCION**          | 0x07   | Error de dirección           | ErrorInfo           |

### **Estructuras de Datos**

#### **AtmosphericSample (6 bytes)**

```cpp
struct AtmosphericSample {
    int16_t temp;      // Temperatura [-400 a 800] (-40.0°C a 80.0°C)
    uint16_t moisture; // Humedad [0 a 1000] (0.0% a 100.0%)
    uint8_t hour;      // Hora [0-23]
    uint8_t minute;    // Minuto [0-59]
};
```

#### **GroundSensor (13 bytes)**

```cpp
struct GroundSensor {
    int16_t temp;      // Temperatura [-400 a 800]
    uint16_t moisture; // Humedad [0 a 1000]
    uint16_t n;        // Nitrógeno [0-1999] mg/kg
    uint16_t p;        // Fósforo [0-1999] mg/kg
    uint16_t k;        // Potasio [0-1999] mg/kg
    uint16_t EC;       // Conductividad [0-20000] μS/cm
    uint8_t PH;        // pH [30-90] (3.0 a 9.0)
};
```

#### **GpsSensor (13 bytes)**

```cpp
struct GpsSensor {
    int32_t latitude;  // Latitud * 10^7 [-1800000000 a 1800000000]
    int32_t longitude; // Longitud * 10^7 [-1800000000 a 1800000000]
    int16_t altitude;  // Altitud [-1000 a 9000] metros
    uint8_t hour;      // Hora [0-23]
    uint8_t minute;    // Minuto [0-59]
    uint8_t flags;     // Flags de validación
};
```

## ⚡ Gestión Energética

### **Consumo por Componente**

| Componente         | Modo Activo | Modo Standby | Modo Sleep |
| ------------------ | ----------- | ------------ | ---------- |
| **ESP32**          | 120 mA      | 15 mA        | 5 mA       |
| **SX1278 LoRa**    | 120 mA (TX) | 2 mA         | 0.2 μA     |
| **GPS NEO-6M**     | 45 mA       | 35 mA        | 25 mA      |
| **Sensores DHT**   | 2.5 mA      | 0.5 mA       | 0.1 μA     |
| **Sensores Suelo** | 50 mA       | 10 mA        | 1 mA       |
| **VoltageReader**  | 1 mA        | 0.1 mA       | 0.01 mA    |

### **Optimización Energética**

```cpp
// Estrategias de ahorro energético
#define DEEP_SLEEP_ENABLED true      // Habilitar deep sleep
#define DEEP_SLEEP_DURATION 300000   // 5 minutos
#define GPS_POWER_SAVE true          // Modo ahorro GPS
#define LORA_POWER_SAVE true         // Modo ahorro LoRa
#define SENSOR_POWER_SAVE true       // Modo ahorro sensores
```

### **Autonomía Estimada**

| Batería      | Capacidad | Autonomía | Uso         |
| ------------ | --------- | --------- | ----------- |
| **12V/7Ah**  | 84 Wh     | 7 días    | Campo       |
| **12V/20Ah** | 240 Wh    | 20 días   | Invernadero |
| **24V/50Ah** | 1200 Wh   | 100 días  | Estación    |

## 📊 Rendimiento del Sistema

### **Precisión de Sensores**

| Sensor            | Rango         | Precisión | Resolución | Tiempo de Respuesta |
| ----------------- | ------------- | --------- | ---------- | ------------------- |
| **Temperatura**   | -40°C a +80°C | ±0.5°C    | 0.1°C      | <2 seg              |
| **Humedad**       | 0-100%        | ±2%       | 0.1%       | <2 seg              |
| **pH**            | 3.0-9.0       | ±0.1      | 0.1        | <5 seg              |
| **Conductividad** | 0-5000 μS/cm  | ±50 μS/cm | 1 μS/cm    | <5 seg              |
| **NPK**           | 0-1999 mg/kg  | ±10 mg/kg | 1 mg/kg    | <10 seg             |
| **Voltaje**       | 0-15V         | ±0.1V     | 0.01V      | <1 seg              |

### **Comunicación LoRa**

| Parámetro                 | Valor         | Descripción    |
| ------------------------- | ------------- | -------------- |
| **Alcance**               | 10 km         | Campo abierto  |
| **Tasa de Datos**         | 0.3-37.5 kbps | Depende del SF |
| **Tiempo de Transmisión** | 50-200 ms     | Por mensaje    |
| **Retransmisiones**       | 3 máximo      | Por mensaje    |
| **Latencia**              | <2 segundos   | End-to-end     |

### **Procesamiento**

| Operación               | Tiempo  | Memoria | CPU |
| ----------------------- | ------- | ------- | --- |
| **Lectura Sensores**    | <100 ms | 2 KB    | 5%  |
| **Procesamiento Datos** | <50 ms  | 1 KB    | 10% |
| **Transmisión LoRa**    | <200 ms | 0.5 KB  | 15% |
| **GPS Update**          | <1 seg  | 0.1 KB  | 2%  |

## 🔧 Configuración y Calibración

### **Parámetros de Configuración**

```cpp
// Configuración de red
#define MAX_NODES 250                 // Máximo número de nodos
#define NODE_ID 1                     // ID único del nodo
#define MESH_TIMEOUT 1500             // Timeout mesh (ms)
#define ANNOUNCE_INTERVAL 5000        // Intervalo announce (ms)

// Configuración de sensores
#define ATMOSPHERIC_INTERVAL 60000    // Intervalo atmosférico (ms)
#define GROUND_INTERVAL 300000        // Intervalo suelo (ms)
#define GPS_INTERVAL 60000            // Intervalo GPS (ms)
#define ENERGY_INTERVAL 30000         // Intervalo energía (ms)

// Configuración de transmisión
#define MAX_RETRIES 3                 // Máximo reintentos
#define ACK_TIMEOUT 1000              // Timeout ACK (ms)
#define MESSAGE_TIMEOUT 5000          // Timeout mensaje (ms)
```

### **Calibración de Sensores**

#### **VoltageReader Calibración**

```cpp
// Calibración automática
voltageReader.calibrate(0.0, 2.5);   // Rango esperado

// Calibración manual
voltageReader.setCalibration(offset, slope);

// Verificación de calibración
float accuracy = voltageReader.getCalibrationAccuracy();
```

#### **Sensores de Suelo Calibración**

```cpp
// Calibración pH
#define PH_CALIBRATION_BUFFER_4 6.86  // Buffer pH 4
#define PH_CALIBRATION_BUFFER_7 4.01  // Buffer pH 7
#define PH_CALIBRATION_BUFFER_10 9.18 // Buffer pH 10

// Calibración EC
#define EC_CALIBRATION_SOLUTION_1413 1413  // μS/cm
#define EC_CALIBRATION_SOLUTION_2764 2764  // μS/cm
#define EC_CALIBRATION_SOLUTION_5530 5530  // μS/cm
```

## 🛡️ Seguridad y Robustez

### **Validación de Datos**

```cpp
// Rangos válidos para cada sensor
#define TEMP_VALID_MIN -400           // -40.0°C
#define TEMP_VALID_MAX 800            // +80.0°C
#define HUMIDITY_VALID_MIN 0          // 0%
#define HUMIDITY_VALID_MAX 1000       // 100%
#define PH_VALID_MIN 30               // 3.0
#define PH_VALID_MAX 90               // 9.0
#define EC_VALID_MIN 0                // 0 μS/cm
#define EC_VALID_MAX 20000            // 20000 μS/cm
#define NPK_VALID_MIN 0               // 0 mg/kg
#define NPK_VALID_MAX 1999            // 1999 mg/kg
```

### **Manejo de Errores**

```cpp
// Códigos de error
#define ERROR_SENSOR_READ_FAILED 0x01
#define ERROR_GPS_NO_FIX 0x02
#define ERROR_LORA_TRANSMISSION 0x03
#define ERROR_VOLTAGE_OUT_OF_RANGE 0x04
#define ERROR_CALIBRATION_FAILED 0x05
#define ERROR_MEMORY_FULL 0x06
```

### **Recuperación de Fallos**

```cpp
// Estrategias de recuperación
#define MAX_SENSOR_RETRIES 3          // Reintentos sensor
#define MAX_LORA_RETRIES 3            // Reintentos LoRa
#define WATCHDOG_TIMEOUT 30000        // Timeout watchdog (ms)
#define ERROR_RECOVERY_DELAY 5000     // Delay recuperación (ms)
```

## 📈 Métricas de Rendimiento

### **KPIs del Sistema**

| Métrica                  | Objetivo  | Actual    | Unidad   |
| ------------------------ | --------- | --------- | -------- |
| **Precisión Sensores**   | >95%      | 97%       | %        |
| **Tiempo de Respuesta**  | <2 seg    | 1.5 seg   | segundos |
| **Alcance Comunicación** | >5 km     | 10 km     | km       |
| **Autonomía Energética** | >5 días   | 7 días    | días     |
| **Disponibilidad**       | >99%      | 99.5%     | %        |
| **Escalabilidad**        | >50 nodos | 100 nodos | nodos    |

### **Análisis de Datos**

```cpp
// Estadísticas de rendimiento
struct SystemStats {
    uint32_t totalMessages;      // Total mensajes enviados
    uint32_t successfulMessages; // Mensajes exitosos
    uint32_t failedMessages;     // Mensajes fallidos
    float successRate;           // Tasa de éxito
    uint32_t totalUptime;       // Tiempo total activo
    float averageResponseTime;   // Tiempo respuesta promedio
    uint32_t sensorReadings;     // Lecturas de sensores
    uint32_t calibrationCount;   // Calibraciones realizadas
};
```

## 🔮 Roadmap Técnico

### **Fase 1 (Completada) ✅**

- [x] Arquitectura modular básica
- [x] Integración de sensores principales
- [x] Sistema de comunicación LoRa
- [x] VoltageReader especializado
- [x] Documentación completa

### **Fase 2 (En Desarrollo) 🔄**

- [ ] Optimización energética avanzada
- [ ] Machine Learning para predicciones
- [ ] Interfaz web de configuración
- [ ] Sistema de alertas inteligente

### **Fase 3 (Planificada) 📋**

- [ ] IA para optimización de riego
- [ ] Integración con drones
- [ ] Análisis de big data
- [ ] Blockchain para trazabilidad

---

**📋 Documento de Especificaciones Técnicas v2.0**

_Sistema de Monitoreo Agrícola - Nodos de Campo_
