// 03:00 16/6/2025
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include "protocol.h"
#include "config.h"
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <DHT.h>       // Incluye la librería DHT (asegúrate de instalarla)
#include <DHT_U.h>     // Incluye la librería de utilidades para DHT (también de Adafruit)
#include "voltage_reader.h" // Incluye la clase VoltageReader para lectura de voltaje
#include "rs485_manager.h"  // Incluye la clase RS485Manager para comunicación con módulo sensor

#define GPS_COORDINATE_HISTORY_SIZE 5 // Número de coordenadas a guardar (puedes ajustar)

/**
 * @class SensorManager
 * @brief Gestiona la adquisición y almacenamiento de datos de sensores ambientales y de suelo en un nodo de la red mesh agrícola.
 *
 * Esta clase abstrae la lógica de lectura de sensores físicos (DHT, NPK, pH, EC, temperatura, humedad, GPS, energía) y almacena las muestras en buffers internos.
 * Permite la actualización periódica de datos, la recolección de muestras para transmisión LoRa y la integración de datos GPS.
 *
 * Forma parte de un nodo autónomo en una red mesh LoRa para monitoreo agrícola, facilitando la toma de decisiones y el envío eficiente de datos al gateway central.
 */
class SensorManager
{
private:
 unsigned long lastSampleTime;           // Tiempo en millis() de la última muestra tomada

  // The serial connection to the GPS device (UART2 HardwareSerial)
  HardwareSerial gpsSerial;
  // The TinyGPSPlus object
  TinyGPSPlus gps;

  DHT dht;

  VoltageReader voltageReader;             ///< Objeto para lectura de voltaje del divisor resistivo
  SoftwareSerial rs485Serial;              ///< SoftwareSerial para RS485
  RS485Manager rs485Manager;               ///< Objeto para comunicación RS485 con módulo sensor
  bool simulationModeEnabled;              ///< Indica si el modo simulación está habilitado

  unsigned long gpsSaveIntervalMs = GPS_SAVE_INTERVAL_MS; // Intervalo de guardado de coordenadas
  unsigned long lastGpsSaveTime = 0;
  struct GpsCoordinate {
      int32_t latitude;
      int32_t longitude;
      uint8_t hour;
      uint8_t minute;
  };
  GpsCoordinate gpsHistory[GPS_COORDINATE_HISTORY_SIZE];
  int gpsHistoryIndex = 0;

  // Método privado para simular o leer sensores atmosfera temp y humedad
  void readAtmosphericSensors();
  // Método privado para simular o leer sensores en tierra npk,humedad,temp,ph,ec(electroconductividad)
  void readGroundSensors();
  // Método privado para simular o leer sensores energeticos voltaje y/o intensidad y/o carga
  void readEnergySensors();
  // Método privado para simular o leer sensores gps
  void readGpsSensors();
  // ve si esta lleno el array sino lo llena con lecturas inmediatas
  void verificFullAtmosSamples();
  void saveGpsCoordinatePeriodically();

public:
  // Almacenamiento de los datos
  // Almacenamiento interno de los datos
  Protocol::AtmosphericSample atmosSamples[NUMERO_MUESTRAS_ATMOSFERICAS]; // Ejemplo: array para 8 muestras
  Protocol::GroundSensor groundData;
  Protocol::GpsSensor gpsData;
  Protocol::EnergyData energyData;
  int atmosSampleCount = 0;               ///< Contador de muestras atmosféricas almacenadas
  // Constructor (opcionalmente inicializar sensores aquí)
  SensorManager();
  void begin();
  // actualiza recurrentemente lecturas atmosfericas
  void update(); // TODO: llamar recurentemente a clase para tomar valores atmosfericos cada x tiempo
  // Método para recolectar datos de los sensores
  void readGroundGpsSensors();
  // Método para recolectar datos de los sensores
  void readSensorsAtmospheric();
  
  /**
   * @brief Obtiene información de debug del VoltageReader.
   * @return String con información detallada del estado del VoltageReader.
   */
  String getVoltageReaderDebugInfo();
  
  /**
   * @brief Realiza calibración del VoltageReader.
   * @param expectedMin Voltaje mínimo esperado del divisor resistivo
   * @param expectedMax Voltaje máximo esperado del divisor resistivo
   */
  void calibrateVoltageReader(float expectedMin, float expectedMax);

  /**
   * @brief Habilita o deshabilita el modo de simulación RS485.
   * @param enabled true para habilitar simulación, false para deshabilitar
   */
  void setSimulationMode(bool enabled);
  
  /**
   * @brief Verifica si el modo de simulación está habilitado.
   * @return true si está habilitado, false en caso contrario
   */
  bool isSimulationModeEnabled() const;
  
  /**
   * @brief Obtiene información de debug del RS485Manager.
   * @return String con información detallada del estado del RS485Manager.
   */
  String getRS485DebugInfo();
  
  /**
   * @brief Imprime información detallada de debugging del GPS.
   * Se llama automáticamente cuando hay errores en la lectura GPS.
   */
  void debugGPS();

  GpsCoordinate getLastGpsCoordinate() const;
};
#endif