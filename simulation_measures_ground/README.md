# Simulación de Sensores de Suelo con RS485

## 📋 Descripción

Este proyecto implementa un **módulo esclavo RS485** que simula sensores de suelo usando potenciómetros. El módulo responde a comandos del nodo principal y envía datos en formato específico.

## 🔧 Hardware Requerido

- **ESP32** (cualquier modelo)
- **Módulo MAX485** para comunicación RS485
- **7 Potenciómetros** (10kΩ recomendado)
- **Cables de conexión**

## 📍 Conexiones

### MAX485

- **DE/RE Pin**: GPIO 2 (control transmisión/recepción)
- **RX Pin**: GPIO 16 (recepción RS485)
- **TX Pin**: GPIO 17 (transmisión RS485)
- **VCC**: 3.3V
- **GND**: GND

### Potenciómetros

- **Temperatura**: GPIO 34
- **Humedad**: GPIO 35
- **Nitrógeno (N)**: GPIO 25
- **Fósforo (P)**: GPIO 26
- **Potasio (K)**: GPIO 27
- **Conductividad (EC)**: GPIO 32
- **pH**: GPIO 33

## 📡 Protocolo de Comunicación

### Comando

```
ILF
```

### Respuesta

```
iTEMP,HUM,PH,EC,N,P,Kf
```

### Ejemplo

```
i25.5,65.2,6.8,1250.0,45,30,25f
```

### Formato de Datos

- **TEMP**: Temperatura en °C (0-50)
- **HUM**: Humedad en % (0-100)
- **PH**: pH del suelo (0-14)
- **EC**: Conductividad en μS/cm (0-2000)
- **N**: Nitrógeno en ppm (0-255)
- **P**: Fósforo en ppm (0-255)
- **K**: Potasio en ppm (0-255)

## ⚙️ Configuración

### Parámetros RS485

- **Baud Rate**: 9600
- **Data Bits**: 8
- **Parity**: None
- **Stop Bits**: 1

### Rangos de Mapeo

- **Potenciómetros**: 0-4095 (12-bit ADC)
- **Temperatura**: -40°C a 50°C
- **Humedad**: 0% a 100%
- **pH**: 0 a 14
- **EC**: 0 a 2000 μS/cm
- **NPK**: 0 a 255 ppm

## 🚀 Funcionamiento

1. **Inicialización**: El módulo se inicia en modo recepción
2. **Espera**: Aguarda comandos del nodo principal
3. **Procesamiento**: Al recibir "ILF":
   - Lee todos los potenciómetros
   - Mapea valores a rangos reales
   - Formatea respuesta según protocolo
   - Envía por RS485
4. **Control DE/RE**:
   - LOW = Modo recepción
   - HIGH = Modo transmisión

## 📁 Estructura del Proyecto

```
src/
├── simulation_measures_ground.ino  # Archivo principal
├── sensor_utils.h                  # Declaraciones de funciones
├── sensor_utils.cpp                # Implementación de sensores
├── ground_sensor_data.h            # Estructuras de datos
└── config.h                        # Configuración centralizada
```

## 🔍 Debug

El sistema incluye mensajes de debug por Serial (115200 baud):

- Lectura de potenciómetros
- Comandos recibidos
- Respuestas enviadas
- Datos mapeados

## ⚠️ Consideraciones

1. **Timing**: Delays incluidos para estabilidad RS485
2. **Buffer**: Limitado a 10 caracteres para comandos
3. **Robustez**: Manejo de comandos no reconocidos
4. **Precisión**: Valores con 1 decimal para temperatura, humedad y pH

## 🔧 Personalización

Para modificar rangos o pines, edita `config.h`:

```cpp
#define TEMP_MIN -40.0
#define TEMP_MAX 50.0
#define RS485_DE_PIN 2
```

## 📊 Ejemplo de Uso

1. Conecta el hardware según el diagrama
2. Sube el código al ESP32
3. Desde el nodo principal, envía: `ILF`
4. Recibe respuesta: `i25.5,65.2,6.8,1250.0,45,30,25f`

## 🛠️ Troubleshooting

- **Sin respuesta**: Verificar conexiones RS485
- **Datos incorrectos**: Calibrar potenciómetros
- **Comunicación errática**: Revisar timing y delays
