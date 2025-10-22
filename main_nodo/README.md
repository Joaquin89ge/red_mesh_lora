# 🌱 SISTEMA DE MONITOREO AGRÍCOLA - NODOS DE CAMPO

## 📋 Descripción

Sistema avanzado de monitoreo agrícola basado en **ESP32** con comunicación **LoRa mesh**, diseñado para la **agricultura de precisión**. Este nodo de campo integra múltiples sensores especializados para proporcionar datos completos sobre condiciones ambientales, suelo y energía.

## 🏗️ Arquitectura del Sistema

```
┌─────────────────────────────────────────────────────────────┐
│                    NODO DE CAMPO ESP32                     │
├─────────────────────────────────────────────────────────────┤
│  📡 LoRa Mesh    📍 GPS      🌡️ DHT      🌱 Sensores     │
│     SX1278      NEO-6M      DHT11/22    NPK/pH/EC        │
├─────────────────────────────────────────────────────────────┤
│  ⚡ VoltageReader    🔋 Energía    📊 Procesamiento       │
│   Divisor Resistivo   Monitoreo     Datos en Tiempo Real  │
└─────────────────────────────────────────────────────────────┘
```

## 🎯 Características Principales

### 🔬 **Sensores Integrados**

- **🌡️ Temperatura Ambiental**: -40°C a +80°C (±0.5°C)
- **💧 Humedad Ambiental**: 0-100% (±2%)
- **🌱 Nutrientes del Suelo**: NPK (0-1999 mg/kg)
- **🧪 pH del Suelo**: 3.0-9.0 (±0.1)
- **⚡ Conductividad**: 0-5000 μS/cm (±50 μS/cm)
- **📍 Geolocalización**: GPS con precisión submétrica
- **🔋 Monitoreo Energético**: Voltaje y corriente en tiempo real

### 📡 **Comunicación Avanzada**

- **LoRa Mesh**: Red de hasta 100 nodos
- **Alcance**: Hasta 10km en campo abierto
- **Protocolo**: Personalizado con enrutamiento dinámico
- **Frecuencia**: 433 MHz (configurable)

### ⚡ **Gestión Energética Inteligente**

- **VoltageReader**: Lectura precisa de divisor resistivo
- **Filtrado**: Media móvil para estabilidad
- **Calibración**: Automática y manual
- **Autonomía**: 7 días con batería 12V/7Ah

## 🛠️ Tecnologías Utilizadas

| Componente           | Tecnología    | Especificación                    |
| -------------------- | ------------- | --------------------------------- |
| **Microcontrolador** | ESP32         | WiFi + Bluetooth + ADC de 12 bits |
| **Comunicación**     | SX1278 LoRa   | 433 MHz, hasta 20 dBm             |
| **GPS**              | NEO-6M        | Precisión submétrica              |
| **Sensores DHT**     | DHT11/DHT22   | Temperatura y humedad             |
| **Sensores Suelo**   | NPK/pH/EC     | Nutrientes y acidez               |
| **Energía**          | VoltageReader | Divisor resistivo 2.5V            |

## 📁 Estructura del Proyecto

```
main_nodo/
├── 📁 src/                    # Código fuente principal
│   ├── main_nodo.ino         # Punto de entrada
│   ├── sensor_manager.h/cpp  # Gestión de sensores
│   ├── voltage_reader.h/cpp  # Lectura de voltaje
│   ├── radio_manager.h/cpp   # Comunicación LoRa
│   ├── app_logic.h/cpp       # Lógica de aplicación
│   ├── node_identity.h/cpp   # Identificación del nodo
│   ├── protocol.h            # Protocolo de comunicación
│   └── config.h              # Configuración global
├── 📁 docs/                  # Documentación completa
│   ├── _build/html/          # Documentación HTML
│   ├── api/                  # API documentation
│   └── overview/             # Visión general del sistema
├── 📁 lib/                   # Librerías personalizadas
├── 📁 include/               # Headers adicionales
└── 📁 test/                  # Pruebas y ejemplos
```

## 🚀 Instalación y Configuración

### **Prerrequisitos**

1. **Arduino IDE** o **PlatformIO**
2. **ESP32 Board Package**
3. **Librerías requeridas**:
   - `RadioHead` (LoRa)
   - `TinyGPSPlus` (GPS)
   - `DHT sensor library` (DHT)
   - `HardwareSerial` (GPS UART2)

### **Configuración Rápida**

```bash
# 1. Clonar el repositorio
git clone https://github.com/Joaquin89ge/main_nodo.git

# 2. Abrir en Arduino IDE o PlatformIO
# 3. Configurar en config.h según tu hardware
# 4. Compilar y subir al ESP32
```

### **Configuración de Hardware**

```cpp
// En config.h - Ajustar según tu conexión
#define RFM95_CS 5          // Pin CS LoRa
#define RFM95_RST 14        // Pin Reset LoRa
#define RFM95_INT 2         // Pin Interrupt LoRa
#define PIN_SENS_DHTT 22    // Pin DHT
#define GPS_RX_UART2 16  // Pin RX GPS (UART2)
#define GPS_TX_UART2 17 // Pin TX GPS (UART2)
#define VOLTAGE_READER_PIN 34 // Pin divisor resistivo
```

## 📊 Especificaciones Técnicas

### **Sensores y Precisión**

| Sensor            | Rango         | Precisión | Resolución |
| ----------------- | ------------- | --------- | ---------- |
| **Temperatura**   | -40°C a +80°C | ±0.5°C    | 0.1°C      |
| **Humedad**       | 0-100%        | ±2%       | 0.1%       |
| **pH**            | 3.0-9.0       | ±0.1      | 0.1        |
| **Conductividad** | 0-5000 μS/cm  | ±50 μS/cm | 1 μS/cm    |
| **NPK**           | 0-1999 mg/kg  | ±10 mg/kg | 1 mg/kg    |
| **Voltaje**       | 0-15V         | ±0.1V     | 0.01V      |

### **Comunicación LoRa**

| Parámetro          | Valor         | Descripción               |
| ------------------ | ------------- | ------------------------- |
| **Frecuencia**     | 433 MHz       | Configurable según región |
| **Potencia**       | Hasta 20 dBm  | Ajustable según alcance   |
| **Modulación**     | LoRa          | SF7-SF12 configurable     |
| **Ancho de Banda** | 125 kHz       | Estándar LoRa             |
| **Tasa de Datos**  | 0.3-37.5 kbps | Depende del SF            |

### **Consumo Energético**

| Modo          | Consumo      | Duración            |
| ------------- | ------------ | ------------------- |
| **Activo**    | 120 mA @ 12V | Transmisión         |
| **Standby**   | 15 mA @ 12V  | Espera              |
| **Sleep**     | 5 mA @ 12V   | Bajo consumo        |
| **Autonomía** | 7 días       | Con batería 12V/7Ah |

## 🔧 Uso del Sistema

### **Inicialización Básica**

```cpp
#include "sensor_manager.h"
#include "radio_manager.h"
#include "app_logic.h"

SensorManager sensors;
RadioManager radio(1); // ID del nodo
AppLogic logic(sensors, radio);

void setup() {
    sensors.begin();
    radio.init();
    logic.begin();
}

void loop() {
    logic.update();
    delay(100);
}
```

### **Lectura de Sensores**

```cpp
// Lectura de sensores atmosféricos
sensors.readSensorsAtmospheric();

// Lectura de sensores de suelo y GPS
sensors.readGroundGpsSensors();

// Acceso a datos
Protocol::AtmosphericSample* atmosData = sensors.atmosSamples;
Protocol::GroundSensor groundData = sensors.groundData;
Protocol::GpsSensor gpsData = sensors.gpsData;
```

### **VoltageReader Avanzado**

```cpp
#include "voltage_reader.h"

VoltageReader voltageReader;

void setup() {
    voltageReader.begin();
    voltageReader.calibrate(0.0, 2.5); // Calibración
}

void loop() {
    float voltage = voltageReader.readVoltage();
    float percentage = voltageReader.readVoltagePercentage();
    int mappedValue = voltageReader.readVoltageMappedInt(0, 100);
}
```

## 📚 Documentación Completa

### **Documentación HTML**

La documentación completa está disponible en formato HTML con:

- **API Reference**: Documentación automática de todas las clases
- **Diagramas**: Arquitectura del sistema y flujo de datos
- **Ejemplos**: Código de ejemplo para cada funcionalidad
- **Guías**: Tutoriales paso a paso

```bash
# Generar documentación
cd docs
python build_docs.py

# Ver documentación
cd _build/html
python -m http.server 8000
# Abrir http://localhost:8000
```

### **Documentación de Código**

Todos los archivos de código incluyen:

- **Comentarios Doxygen**: Documentación automática
- **Especificaciones técnicas**: Detalles de implementación
- **Ejemplos de uso**: Código de ejemplo
- **Guías de configuración**: Parámetros ajustables

## 🔄 Integración con Red Mesh

Este nodo forma parte de un sistema más grande:

```
┌─────────────────┐    LoRa Mesh    ┌─────────────────┐
│   NODO CAMPO    │◄──────────────►│  GATEWAY CENTRAL│
│   (main_nodo)   │                │ (main_gateway)  │
└─────────────────┘                └─────────────────┘
         │                                   │
         ▼                                   ▼
   ┌─────────────┐                   ┌─────────────┐
   │   Sensores  │                   │   Dashboard │
   │  Agrícolas  │                   │     Web     │
   └─────────────┘                   └─────────────┘
```

## 🎯 Casos de Uso

### **Monitoreo de Cultivos**

- Seguimiento de humedad del suelo
- Control de temperatura ambiental
- Optimización de riego automático
- Predicción de rendimiento

### **Gestión de Invernaderos**

- Control climático automático
- Monitoreo de nutrientes
- Alertas de condiciones críticas
- Registro histórico de datos

### **Agricultura de Precisión**

- Mapeo de variabilidad del suelo
- Aplicación variable de insumos
- Optimización de recursos
- Análisis de tendencias

## 🔧 Desarrollo y Contribución

### **Estructura Modular**

El código sigue principios **SOLID**:

- **Single Responsibility**: Cada clase tiene una responsabilidad específica
- **Open/Closed**: Extensible sin modificar código existente
- **Liskov Substitution**: Interfaces bien definidas
- **Interface Segregation**: Interfaces específicas
- **Dependency Inversion**: Dependencias invertidas

### **Patrones de Diseño**

- **Manager Pattern**: SensorManager, RadioManager
- **Observer Pattern**: Notificaciones de eventos
- **Factory Pattern**: Creación de objetos
- **Strategy Pattern**: Diferentes algoritmos de procesamiento

## 📈 Roadmap

### **Fase 1 (Completada) ✅**

- [x] Arquitectura modular básica
- [x] Integración de sensores principales
- [x] Sistema de comunicación LoRa
- [x] VoltageReader especializado
- [x] Documentación completa con Sphinx

### **Fase 2 (En Desarrollo) 🔄**

- [ ] Optimización de consumo energético
- [ ] Interfaz web para configuración
- [ ] Sistema de alertas avanzado
- [ ] Integración con IA para predicciones

### **Fase 3 (Planificada) 📋**

- [ ] Interfaz móvil para monitoreo
- [ ] Integración con sistemas de riego
- [ ] Análisis avanzado de datos
- [ ] Machine Learning para optimización

## 🤝 Contribución

### **Cómo Contribuir**

1. **Fork** el repositorio
2. **Crea** una rama para tu feature (`git checkout -b feature/nueva-funcionalidad`)
3. **Commit** tus cambios (`git commit -am 'Añadir nueva funcionalidad'`)
4. **Push** a la rama (`git push origin feature/nueva-funcionalidad`)
5. **Crea** un Pull Request

### **Estándares de Código**

- **Documentación**: Comentarios Doxygen en todos los headers
- **Nomenclatura**: camelCase para variables, PascalCase para clases
- **Indentación**: 2 espacios
- **Comentarios**: En español para consistencia

## 📞 Soporte

### **Documentación**

- **API Reference**: `docs/_build/html/api/`
- **Guías de Usuario**: `docs/_build/html/overview/`
- **Ejemplos**: `docs/_build/html/examples/`

### **Contacto**

- **Issues**: GitHub Issues para reportar bugs
- **Discussions**: GitHub Discussions para preguntas
- **Wiki**: Documentación adicional y FAQs

## 📄 Licencia

Este proyecto está bajo la licencia **MIT**. Ver el archivo `LICENSE` para más detalles.

---

**🌱 Desarrollado con ❤️ para la agricultura de precisión**

_Sistema de Monitoreo Agrícola - Nodos de Campo v2.0_
