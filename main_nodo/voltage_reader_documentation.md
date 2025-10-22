# VoltageReader - Documentación Completa

## Descripción General

La clase `VoltageReader` es una implementación eficiente y precisa para leer voltajes de un divisor resistivo conectado al pin 34 del ESP32. Está optimizada para señales de 2.5V a 0V con configuración ADC_ATTEN_DB_11 para el rango 0-2.6V.

## Características Principales

### ✅ Configuración Optimizada
- **Pin:** 34 (configurable desde `config.h`)
- **ADC:** 12 bits de resolución
- **Atenuación:** ADC_ATTEN_DB_11 (rango 0-2.6V)
- **Voltaje de referencia:** 3.3V

### ✅ Funciones Eficientes
- **Filtrado:** Media móvil de 8 muestras
- **Mapeo:** Uso de funciones `map()` de Arduino
- **Calibración:** Automática y manual
- **Validación:** Verificación de rangos válidos

### ✅ Precisión
- **Resolución:** 12 bits (0-4095)
- **Rango de entrada:** 0.0V a 2.5V
- **Filtrado:** Reduce ruido y estabiliza lecturas
- **Calibración:** Compensa no-linealidades del ADC

## Configuración en config.h

```cpp
// --- Configuración VoltageReader (Divisor Resistivo) ---
#define VOLTAGE_READER_PIN 34             ///< Pin analógico
#define VOLTAGE_READER_ADC_RESOLUTION 12  ///< Resolución ADC
#define VOLTAGE_READER_ADC_ATTEN ADC_ATTEN_DB_11 ///< Atenuación ADC
#define VOLTAGE_READER_VREF 3.3           ///< Voltaje de referencia
#define VOLTAGE_READER_MAX_INPUT_VOLTAGE 2.5 ///< Voltaje máximo
#define VOLTAGE_READER_MIN_INPUT_VOLTAGE 0.0  ///< Voltaje mínimo
#define VOLTAGE_READER_FILTER_SAMPLES 8   ///< Muestras para filtrado
#define VOLTAGE_READER_CALIBRATION_OFFSET 0.0 ///< Offset por defecto
#define VOLTAGE_READER_CALIBRATION_SLOPE 1.0  ///< Pendiente por defecto
```

## Uso Básico

### Inicialización
```cpp
#include "voltage_reader.h"

VoltageReader voltageReader;

void setup() {
    voltageReader.begin();
}
```

### Lectura de Voltaje
```cpp
void loop() {
    float voltage = voltageReader.readVoltage();
    Serial.println("Voltaje: " + String(voltage, 3) + "V");
    delay(1000);
}
```

## Métodos Principales

### Lectura Básica
```cpp
float readVoltage();                    // Voltaje en voltios (0.0-2.5V)
uint16_t readRawValue();               // Valor raw del ADC (0-4095)
float readVoltagePercentage();         // Porcentaje (0.0-100.0%)
```

### Mapeo con map()
```cpp
float readVoltageMapped(float min, float max);     // Mapeo a float
int readVoltageMappedInt(int min, int max);        // Mapeo a entero
```

### Calibración
```cpp
void setCalibration(float offset, float slope);    // Calibración manual
void calibrate(float expectedMin, float expectedMax); // Calibración automática
```

### Validación y Debug
```cpp
bool isReadingValid();                 // Verificar rango válido
String getDebugInfo();                 // Información de debug
```

## Ejemplos de Aplicación

### 1. Medir Nivel de Batería
```cpp
float batteryLevel = voltageReader.readVoltagePercentage();
Serial.println("Batería: " + String(batteryLevel, 1) + "%");
```

### 2. Sensor de Humedad del Suelo
```cpp
float soilMoisture = voltageReader.readVoltageMapped(0.0, 100.0);
Serial.println("Humedad: " + String(soilMoisture, 1) + "%");
```

### 3. Sensor de Luz
```cpp
int lightLevel = voltageReader.readVoltageMappedInt(0, 1023);
Serial.println("Luz: " + String(lightLevel));
```

### 4. Detectar Umbrales
```cpp
float voltage = voltageReader.readVoltage();
if (voltage < 0.5) {
    Serial.println("¡Alerta! Voltaje muy bajo");
} else if (voltage > 2.0) {
    Serial.println("¡Alerta! Voltaje muy alto");
}
```

## Calibración

### Calibración Automática
```cpp
// Calibrar para rango esperado 0.0V a 2.5V
voltageReader.calibrate(0.0, 2.5);
```

### Calibración Manual
```cpp
// Configurar offset y pendiente personalizados
voltageReader.setCalibration(0.1, 0.98);
```

## Información de Debug

```cpp
String debugInfo = voltageReader.getDebugInfo();
Serial.println(debugInfo);
```

**Salida de ejemplo:**
```
=== VoltageReader Debug Info ===
Pin: 34
ADC Resolution: 12 bits
ADC Attenuation: 11dB
VRef: 3.3V
Raw ADC Value: 2048
Voltage: 1.250V
Percentage: 50.0%
Calibration Offset: 0.0
Calibration Slope: 1.0
Input Range: 0.0V to 2.5V
Filter Samples: 8
Filter Initialized: Yes
Valid Reading: Yes
================================
```

## Optimizaciones Implementadas

### 1. Uso de map() de Arduino
```cpp
// Antes (cálculo manual)
float percentage = ((MAX_INPUT_VOLTAGE - voltage) / (MAX_INPUT_VOLTAGE - MIN_INPUT_VOLTAGE)) * 100.0;

// Ahora (función map() de Arduino)
return map(voltage, MAX_INPUT_VOLTAGE, MIN_INPUT_VOLTAGE, 0.0, 100.0);
```

### 2. Configuración ADC_ATTEN_DB_11
```cpp
// Configurar atenuación para rango 0-2.6V
analogSetAttenuation(VOLTAGE_READER_ADC_ATTEN);
```

### 3. Filtrado Eficiente
```cpp
// Media móvil de 8 muestras (optimizado)
static const uint8_t FILTER_SAMPLES = 8;
```

### 4. Configuración Centralizada
```cpp
// Todas las constantes en config.h
#define VOLTAGE_READER_PIN 34
#define VOLTAGE_READER_ADC_ATTEN ADC_ATTEN_DB_11
```

## Ventajas de la Implementación

### ✅ Eficiencia
- Uso de funciones nativas de Arduino (`map()`)
- Filtrado optimizado (8 muestras)
- Configuración centralizada

### ✅ Precisión
- ADC_ATTEN_DB_11 para rango específico
- Calibración automática y manual
- Validación de rangos

### ✅ Facilidad de Uso
- Interfaz simple y clara
- Ejemplos completos
- Documentación detallada

### ✅ Flexibilidad
- Mapeo a cualquier rango
- Calibración personalizable
- Configuración desde config.h

## Integración con el Sistema

La clase `VoltageReader` se integra perfectamente con el sistema de sensores agrícolas:

```cpp
// En SensorManager
VoltageReader batteryMonitor;
VoltageReader soilMoistureSensor;

void setup() {
    batteryMonitor.begin();
    soilMoistureSensor.begin();
}

void readSensors() {
    float batteryLevel = batteryMonitor.readVoltagePercentage();
    float soilMoisture = soilMoistureSensor.readVoltageMapped(0.0, 100.0);
    
    // Enviar datos al sistema mesh
    sendSensorData(batteryLevel, soilMoisture);
}
```

## Consideraciones Técnicas

### Rango de Voltaje
- **Entrada:** 2.5V a 0V (divisor resistivo)
- **ADC:** 0V a 2.6V (ADC_ATTEN_DB_11)
- **Mapeo:** 2.5V = 0%, 0V = 100%

### Precisión
- **Resolución:** 12 bits = 4096 niveles
- **Rango efectivo:** 0-2.5V = 3125 niveles
- **Precisión teórica:** ~0.8mV

### Filtrado
- **Tipo:** Media móvil
- **Muestras:** 8 (configurable)
- **Efecto:** Reduce ruido y estabiliza lecturas

### Calibración
- **Automática:** Basada en valores esperados
- **Manual:** Offset y pendiente personalizados
- **Efecto:** Compensa no-linealidades del ADC

## Conclusión

La clase `VoltageReader` proporciona una solución eficiente, precisa y fácil de usar para la lectura de divisores resistivos en el ESP32. Su implementación optimizada con funciones nativas de Arduino y configuración centralizada la hace ideal para aplicaciones agrícolas que requieren mediciones confiables de voltaje. 