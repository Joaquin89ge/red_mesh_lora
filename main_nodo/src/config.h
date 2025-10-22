
#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file config.h
 * @brief Define parámetros de configuración globales para el nodo sensor mesh agrícola.
 *
 * Este archivo contiene constantes, pines, intervalos y parámetros clave que determinan el comportamiento y la integración de hardware y software en el nodo.
 * Modificar estos valores permite adaptar el sistema a diferentes topologías, sensores y requerimientos energéticos.
 *
 * El uso de DEBUG_MODE permite reducir los tiempos de espera y los intervalos de muestreo para pruebas rápidas durante el desarrollo.
 * En modo normal, los intervalos son los recomendados para operación real en campo.
 */

// --- Modo Debug ---
/**
 * @def DEBUG_MODE
 * @brief Si está definido, activa el modo debug con intervalos cortos para pruebas.
 */
#define DEBUG_MODE

#ifdef DEBUG_MODE
/**
 * @def DEBUG_PRINTF
 * @brief Macro para imprimir mensajes de debug con formato.
 */
#define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
/**
 * @def INTERVALOHELLO
 * @brief Intervalo en milisegundos para el envío de mensajes HELLO en modo debug (10 segundos).
 */
#define INTERVALOHELLO 10000
/**
 * @def NUMERO_MUESTRAS_ATMOSFERICAS
 * @brief Número de muestras atmosféricas a almacenar antes de enviar en modo debug.
 */
#define NUMERO_MUESTRAS_ATMOSFERICAS 8
#define SAMPLEINTERVALMSATMOSPHERIC 20000// Intervalo de muestreo en milisegundos cada 0.2 minuto un poco menos por las dudas

#define GPS_SAVE_INTERVAL_MS 20000 // Intervalo de guardado de coordenadas GPS en ms (ejemplo: 20 segundos)


#else
/**
 * @def DEBUG_PRINTF
 * @brief Macro vacía para deshabilitar debug en modo normal.
 */
#define DEBUG_PRINTF(...)
/**
 * @def INTERVALOHELLO
 * @brief Intervalo en milisegundos para el envío de mensajes HELLO en modo normal (60 segundos).
 */
#define INTERVALOHELLO 60000
/**
 * @def NUMERO_MUESTRAS_ATMOSFERICAS
 * @brief Número de muestras atmosféricas a almacenar antes de enviar en modo normal.
 */
#define NUMERO_MUESTRAS_ATMOSFERICAS 8

#define SAMPLEINTERVALMSATMOSPHERIC 35000// Intervalo de muestreo en milisegundos cada 0.35 minuto un poco menos por las dudas

#define GPS_SAVE_INTERVAL_MS 3600000 // Intervalo de guardado de coordenadas GPS en ms (ejemplo: 60 minuto)


#endif

// --- Parámetros de red y mensajes ---
// #define RH_MESH_MAX_MESSAGE_LEN 50  // Comentado para evitar conflicto con RadioHead
#define MAC_STR_LEN_WITH_NULL 18



// --- Configuración de reset automático del módulo radio ---
/**
 * @def RADIO_MAX_FAILURES
 * @brief Número máximo de fallos consecutivos antes de resetear el módulo radio.
 */
#define RADIO_MAX_FAILURES 10   // 10 fallos consecutivos tratar de que sea numero alto.
                // ya que reseteo muy seguido puede causar corrompimiento de clase rhMesh

/**
 * @def RADIO_RESET_DELAY_MS
 * @brief Tiempo de espera después del reset del módulo radio en milisegundos.
 */
#define RADIO_RESET_DELAY_MS 1000 // 1 segundo  


// --- Pines de hardware ---
/**
 * @def RFM95_CS
 * @brief Pin Chip Select para el módulo LoRa RFM95.
 */
#define RFM95_CS 5

/**
 * @def RFM95_INT
 * @brief Pin de Interrupción para el módulo LoRa RFM95.
 * Este pin es utilizado por el módulo RFM95 para señalar eventos (por ejemplo, paquete recibido) al microcontrolador.
 */
#define RFM95_INT 26

/**
 * @def RFM95_RST
 * @brief Pin Reset para el módulo LoRa RFM95.
 */
#define RFM95_RST 27

/**
 * @def VSPI_SCK
 * @brief Pin SPI SCK para comunicación con RFM95.
 */
#define VSPI_SCK 18

/**
 * @def VSPI_MISO
 * @brief Pin SPI MISO para comunicación con RFM95.
 */
#define VSPI_MISO 19

/**
 * @def VSPI_MOSI
 * @brief Pin SPI MOSI para comunicación con RFM95.
 */
#define VSPI_MOSI 23

/**
 * @def VSPI_SS
 * @brief Pin SPI SS para comunicación con RFM95.
 */
#define VSPI_SS 5

// --- Configuración GPS (UART2 HardwareSerial) ---
/**
 * @def GPS_RX_UART2
 * @brief Pin RX del ESP32 para GPS usando UART2 (conecta a TX del GPS).
 */
#define GPS_RX_UART2 16

/**
 * @def GPS_TX_UART2
 * @brief Pin TX del ESP32 para GPS usando UART2 (conecta a RX del GPS).
 */
#define GPS_TX_UART2 17

/**
 * @def GPS_BAUDRATE
 * @brief Velocidad de comunicación GPS en baudios.
 */
#define GPS_BAUDRATE 9600

// --- Configuración RS485 (SoftwareSerial) ---
/**
 * @def RS485_RX_SOFTWARE
 * @brief Pin RX del ESP32 para RS485 usando SoftwareSerial (conecta a TX del módulo RS485).
 */
#define RS485_RX_SOFTWARE 4

/**
 * @def RS485_TX_SOFTWARE
 * @brief Pin TX del ESP32 para RS485 usando SoftwareSerial (conecta a RX del módulo RS485).
 */
#define RS485_TX_SOFTWARE 15


/**
 * @def RS485_RE_DE
 * @brief Pin para control de dirección RE/DE del transceptor RS485.
 * Un solo pin controla tanto DE como RE (HIGH=Transmitir, LOW=Recibir).
 */
#define RS485_RE_DE 2

// --- Configuración RS485 para comunicación con módulo sensor ---
/**
 * @def RS485_BAUDRATE
 * @brief Velocidad de comunicación RS485 en baudios.
 */
#define RS485_BAUDRATE 9600


/**
 * @def RS485_TIMEOUT_MS
 * @brief Timeout para comunicación RS485 en milisegundos.
 */
#define RS485_TIMEOUT_MS 1000

/**
 * @def RS485_RETRY_COUNT
 * @brief Número de reintentos para comunicación RS485.
 */
#define RS485_RETRY_COUNT 2

// Delays para RS485 binario
#ifndef RS485_TX_DELAY
#define RS485_TX_DELAY 3   // ms
#endif
#ifndef RS485_RESPONSE_DELAY
#define RS485_RESPONSE_DELAY 3 // ms
#endif

/**
 * @def RS485_COMMAND_REQUEST_DATA
 * @brief Comando para solicitar datos del módulo sensor.
 */
#define RS485_COMMAND_REQUEST_DATA 0x01

/**
 * @def RS485_MODE_READ
 * @brief Modo lectura para RS485 (DE=0, RE=0).
 */
#define RS485_MODE_READ 0

/**
 * @def RS485_MODE_WRITE
 * @brief Modo escritura para RS485 (DE=1, RE=1).
 */
#define RS485_MODE_WRITE 1

/**
 * @def PIN_SENS_DHTT
 * @brief Pin digital del ESP32 al que conectas el pin de datos del DHT11.
 */
#define PIN_SENS_DHTT 22

/**
 * @def DHTTYPE
 * @brief Define el tipo de sensor: DHT11 o DHT22.
 */
#define DHTTYPE DHT11

// --- Pin para el Divisor Resistivo (medición de batería) ---
/**
 * @def BATTERY_VOLTAGE_ADC_PIN
 * @brief Pin analógico para leer el voltaje de la batería.
 */
#define BATTERY_VOLTAGE_ADC_PIN 34

// --- Valores de Error Comunes para Sensores ---
/**
 * @def SENSOR_ERROR_TEMP
 * @brief Valor de error para temperatura (-9999).
 */
#define SENSOR_ERROR_TEMP -9999

/**
 * @def SENSOR_ERROR_MOISTURE
 * @brief Valor de error para humedad (9999).
 */
#define SENSOR_ERROR_MOISTURE 9999

/**
 * @def SENSOR_ERROR_LATITUDE
 * @brief Valor de error para latitud (2000000000L).
 */
#define SENSOR_ERROR_LATITUDE 2000000000L

/**
 * @def SENSOR_ERROR_LONGITUDE
 * @brief Valor de error para longitud (2000000000L).
 */
#define SENSOR_ERROR_LONGITUDE 2000000000L

/**
 * @def SENSOR_ERROR_ALTITUDE
 * @brief Valor de error para altitud (-9999).
 */
#define SENSOR_ERROR_ALTITUDE -9999

/**
 * @def SENSOR_ERROR_TIME_COMPONENT
 * @brief Valor de error para componentes de tiempo (255).
 */
#define SENSOR_ERROR_TIME_COMPONENT 255

/**
 * @def SENSOR_ERROR_DATE_COMPONENT
 * @brief Valor de error para componentes de fecha (255).
 */
#define SENSOR_ERROR_DATE_COMPONENT 255

/**
 * @def SENSOR_ERROR_YEAR
 * @brief Valor de error para año (0xFFFF).
 */
#define SENSOR_ERROR_YEAR 0xFFFF

/**
 * @def SENSOR_ERROR_PH
 * @brief Valor de error para pH (255 - máximo para uint8_t).
 */
#define SENSOR_ERROR_PH 255

/**
 * @def SENSOR_ERROR_EC
 * @brief Valor de error para conductividad eléctrica (0xFFFF).
 */
#define SENSOR_ERROR_EC 0xFFFF

/**
 * @def SENSOR_ERROR_NPK
 * @brief Valor de error para NPK (0xFFFF).
 */
#define SENSOR_ERROR_NPK 0xFFFF

// --- Configuración de Task Watchdog Timer (TWDT) ---
/**
 * @def TWDT_TIMEOUT_SECONDS
 * @brief Timeout del TWDT en segundos.
 */
#define TWDT_TIMEOUT_SECONDS 14

/**
 * @def TWDT_RESET_INTERVAL_MS
 * @brief Intervalo para resetear TWDT en milisegundos.
 */
#define TWDT_RESET_INTERVAL_MS 7000

/**
 * @def TWDT_ENABLE_PANIC
 * @brief Habilitar reinicio automático del ESP32 si TWDT expira.
 */
#define TWDT_ENABLE_PANIC true

// === Parámetros de mapeo y límites de voltaje para VoltageReader ===
#define VOLTAGE_READER_MAX_INPUT_VOLTAGE 2.5f
#define VOLTAGE_READER_MIN_INPUT_VOLTAGE 0.0f
#define VOLTAGE_READER_SYSTEM_VOLTAGE_MAX 5.0f  // Valor máximo del sistema para mapear
#define VOLTAGE_READER_SYSTEM_VOLTAGE_MIN 0.0f   // Valor mínimo del sistema para mapear

// === Parámetro de corriente simulada en SensorManager ===
#define SENSOR_MANAGER_CURRENT_DIVISOR 10.0f

// --- Configuración para modo simulación RS485 ---
/**
 * @def SENSOR_SIMULATION_ENABLED
 * @brief Flag para habilitar/deshabilitar modo simulación RS485 (1=habilitado, 0=deshabilitado).
 */
#define SENSOR_SIMULATION_ENABLED 0


#endif // CONFIG_H
