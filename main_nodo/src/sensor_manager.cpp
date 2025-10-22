// 03:00 16/6/2025
#include "sensor_manager.h"

// Constructor (opcionalmente inicializar sensores aquí)
SensorManager::SensorManager()
    : gpsSerial(2), dht(PIN_SENS_DHTT, DHTTYPE), gps() // UART2 para GPS
    , rs485Serial(RS485_RX_SOFTWARE, RS485_TX_SOFTWARE) // SoftwareSerial para RS485
    , rs485Manager(&rs485Serial, RS485_RE_DE) // Usar SoftwareSerial con pin de control RS485
{
  simulationModeEnabled = (SENSOR_SIMULATION_ENABLED == 1); // Inicializar según flag de config
  // begin();
}
void SensorManager::begin()
{
  Serial.print("constructor sensor manage");
  gpsSerial.begin(GPS_BAUDRATE);
  dht.begin();
  voltageReader.begin(); // Inicializar VoltageReader
  
  // Siempre inicializar RS485Manager
  if (rs485Manager.begin()) {
    DEBUG_PRINTF("SensorManager: RS485 inicializado correctamente\n");
  } else {
    DEBUG_PRINTF("SensorManager: Error inicializando RS485\n");
  }
  
  // Handshake RS485 - verificar que el esclavo está en línea
  DEBUG_PRINTF("SensorManager: Verificando conexión RS485...\n");
  int attempts = 0;
  const int MAX_ATTEMPTS = 10;
  while (!rs485Manager.sendPing() && attempts < MAX_ATTEMPTS) {
    attempts++;
    DEBUG_PRINTF("  Intento %d/%d...\n", attempts, MAX_ATTEMPTS);
    delay(500);
  }
  
  if (attempts < MAX_ATTEMPTS) {
    DEBUG_PRINTF("SensorManager: RS485 esclavo respondió OK\n");
  } else {
    DEBUG_PRINTF("SensorManager: ADVERTENCIA - RS485 esclavo no responde\n");
  }
}

// Método privado para simular o leer sensores en tierra npk,humedad,temp,ph,ec(electroconductividad)
void SensorManager::readGroundSensors()
{
  if  (rs485Manager.isAvailable()) {
    // Solicitar datos del módulo sensor externo
    if (rs485Manager.requestSensorData(groundData)) {
      DEBUG_PRINTF("SensorManager: Datos de suelo recibidos por RS485\n");
      DEBUG_PRINTF("  pH: %d, EC: %d, Temp: %d, Humedad: %d\n", 
                    groundData.PH, groundData.EC, groundData.temp, groundData.moisture);
      DEBUG_PRINTF("  N: %d, P: %d, K: %d\n", 
                    groundData.n, groundData.p, groundData.k);
    } else {
      DEBUG_PRINTF("SensorManager: Error obteniendo datos por RS485 (Error: %d, Intentos: %d), usando valores de error\n", 
                    rs485Manager.getLastError(), rs485Manager.getFailedAttempts());
      // Usar valores de error si falla la comunicación
      groundData.temp = SENSOR_ERROR_TEMP;
      groundData.moisture = SENSOR_ERROR_MOISTURE;
      groundData.n = SENSOR_ERROR_NPK;
      groundData.p = SENSOR_ERROR_NPK;
      groundData.k = SENSOR_ERROR_NPK;
      groundData.EC = SENSOR_ERROR_EC;
      groundData.PH = SENSOR_ERROR_PH;
    }
  } 
}

bool bufferLlenoAdvertido = false; // Bandera global o estática para advertencia

void SensorManager::readAtmosphericSensors()
{
  if (atmosSampleCount < NUMERO_MUESTRAS_ATMOSFERICAS)
  {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t))
    {
      Serial.println("ERROR: ¡Fallo al leer del sensor DHT11!");
      atmosSamples[atmosSampleCount].temp = SENSOR_ERROR_TEMP;
      atmosSamples[atmosSampleCount].moisture = SENSOR_ERROR_MOISTURE;
    }
    else
    {
      atmosSamples[atmosSampleCount].temp = (int16_t)(t * 10.0);
      atmosSamples[atmosSampleCount].moisture = (uint16_t)(h * 10.0);
    }
    if (gps.time.isValid())
    {
      atmosSamples[atmosSampleCount].hour = gps.time.hour();
      atmosSamples[atmosSampleCount].minute = gps.time.minute();
    }
    else
    {
      Serial.println("ADVERTENCIA: Tiempo GPS no válido para muestra atmosférica.");
      debugGPS(); // Llamar al debugging detallado
      atmosSamples[atmosSampleCount].hour = SENSOR_ERROR_TIME_COMPONENT;
      atmosSamples[atmosSampleCount].minute = SENSOR_ERROR_TIME_COMPONENT;
    }

    atmosSampleCount++;
    Serial.print("DEBUG: Muestra atmosferica #");
    Serial.print(String(atmosSampleCount));
    Serial.println(" guardada.");
  }
  else
  {
    // Si el buffer está lleno, sobrescribe la última posición
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t))
    {
      atmosSamples[NUMERO_MUESTRAS_ATMOSFERICAS - 1].temp = SENSOR_ERROR_TEMP;
      atmosSamples[NUMERO_MUESTRAS_ATMOSFERICAS - 1].moisture = SENSOR_ERROR_MOISTURE;
      atmosSamples[NUMERO_MUESTRAS_ATMOSFERICAS - 1].hour = SENSOR_ERROR_TIME_COMPONENT;
      atmosSamples[NUMERO_MUESTRAS_ATMOSFERICAS - 1].minute = SENSOR_ERROR_TIME_COMPONENT;
    }
    else
    {
      atmosSamples[NUMERO_MUESTRAS_ATMOSFERICAS - 1].temp = (int16_t)(t * 10.0);
      atmosSamples[NUMERO_MUESTRAS_ATMOSFERICAS - 1].moisture = (uint16_t)(h * 10.0);
      if (gps.time.isValid())
      {
        atmosSamples[NUMERO_MUESTRAS_ATMOSFERICAS - 1].hour = gps.time.hour();
        atmosSamples[NUMERO_MUESTRAS_ATMOSFERICAS - 1].minute = gps.time.minute();
      }
      else
      {
        Serial.println("ADVERTENCIA: Tiempo GPS no válido para muestra atmosférica (sobrescritura).");
        debugGPS(); // Llamar al debugging detallado
        atmosSamples[NUMERO_MUESTRAS_ATMOSFERICAS - 1].hour = SENSOR_ERROR_TIME_COMPONENT;
        atmosSamples[NUMERO_MUESTRAS_ATMOSFERICAS - 1].minute = SENSOR_ERROR_TIME_COMPONENT;
      }
    }
    // No imprimir advertencia ni bloquear
  }
}
// Método privado para simular o leer sensores en tierra npk,humedad,temp,ph,ec(electroconductividad)

void SensorManager::readGpsSensors()
{
  // Reinicia las flags y los valores a error al inicio de cada lectura
  gpsData.flags = 0;                        // Todas las flags a 0 (inválidas por defecto)
  gpsData.latitude = SENSOR_ERROR_LATITUDE; // Usar float para error si la estructura es float
  gpsData.longitude = SENSOR_ERROR_LONGITUDE;
  gpsData.altitude = SENSOR_ERROR_ALTITUDE;

  gpsData.hour = SENSOR_ERROR_TIME_COMPONENT;
  gpsData.minute = SENSOR_ERROR_TIME_COMPONENT;

  bool hasErrors = false; // Flag para detectar si hay errores

  // --- Procesar Ubicación (Latitud y Longitud) ---
  if (gps.location.isValid())
  {
    gpsData.latitude = gps.location.lat();
    gpsData.longitude = gps.location.lng();
    gpsData.flags |= 0x01; // Establece el bit0: Ubicación válida
    Serial.printf("GPS - Ubicación VÁLIDA: Lat=%.7f, Lon=%.7f\n", 
                  gpsData.latitude, gpsData.longitude);
  }
  else
  {
    hasErrors = true;
    Serial.println("GPS - ERROR: Ubicación no válida");
  }

  // --- Procesar Altitud ---
  if (gps.altitude.isValid())
  {
    gpsData.altitude = gps.altitude.meters();
    gpsData.flags |= 0x02; // Establece el bit1: Altitud válida
    Serial.printf("GPS - Altitud VÁLIDA: %.2f metros\n", gpsData.altitude);
  }
  else
  {
    hasErrors = true;
    Serial.println("GPS - ERROR: Altitud no válida");
  }

  // --- Procesar Fecha y Hora ---
  if (gps.date.isValid() && gps.time.isValid())
  {
    // Hora UTC del GPS
    gpsData.hour = gps.time.hour();
    gpsData.minute = gps.time.minute();

    // --- OPCIONAL: Ajuste a la hora local de Buenos Aires (UTC-3) ---
    // Si necesitas la hora ajustada para el almacenamiento final, aplica aquí.
    // Para un manejo robusto de la fecha (día, mes, año) al cruzar la medianoche,
    // se recomienda usar una librería de tiempo más completa o convertir a timestamp Unix.
    /*
    int adjustedHour = gpsData.hour - 3;
    if (adjustedHour < 0) {
      adjustedHour += 24; // Pasa al día anterior
      // Lógica para decrementar el día, mes, y año si es necesario
    }
    gpsData.hour = adjustedHour;
    */
    // ---------------------------------------------------------------

    gpsData.flags |= 0x04; // Establece el bit2: Fecha y Hora válidas
    Serial.printf("GPS - Fecha/Hora VÁLIDAS: %02d:%02d:%02d UTC\n", 
                  gpsData.hour, gpsData.minute, gps.time.second());
  }
  else
  {
    hasErrors = true;
    Serial.println("GPS - ERROR: Fecha/Hora no válidas");
  }
  
  // Si hay errores, llamar al debugging detallado
  if (hasErrors) {
    debugGPS();
  }
}

void SensorManager::debugGPS() {
  Serial.println("=== DEBUG GPS DETALLADO ===");
  
  // Información general del GPS
  Serial.printf("GPS - Caracteres procesados: %d\n", gps.charsProcessed());
  Serial.printf("GPS - Sentencias con fix: %d\n", gps.sentencesWithFix());
  Serial.printf("GPS - Checksums fallidos: %d\n", gps.failedChecksum());
  Serial.printf("GPS - Checksums válidos: %d\n", gps.passedChecksum());
  
  // Información de satélites
  Serial.printf("GPS - Satélites visibles: %d\n", gps.satellites.value());
  Serial.printf("GPS - HDOP: %.1f\n", gps.hdop.hdop());
  
  // Estado de fix
  if (gps.location.isValid()) {
    Serial.println("GPS - Estado: FIX VÁLIDO");
  } else {
    Serial.println("GPS - Estado: SIN FIX");
  }
  
  // Información de tiempo
  Serial.printf("GPS - Tiempo desde inicio: %lu ms\n", millis());
  Serial.printf("GPS - Última actualización: %d ms\n", gps.location.age());
  
  // Información de velocidad y curso (si está disponible)
  if (gps.speed.isValid()) {
    Serial.printf("GPS - Velocidad: %.2f km/h\n", gps.speed.kmph());
  }
  if (gps.course.isValid()) {
    Serial.printf("GPS - Curso: %.1f°\n", gps.course.deg());
  }
  
  // Información de fecha y hora
  if (gps.date.isValid()) {
    Serial.printf("GPS - Fecha: %02d/%02d/%02d\n", 
                  gps.date.month(), gps.date.day(), gps.date.year());
  }
  if (gps.time.isValid()) {
    Serial.printf("GPS - Hora: %02d:%02d:%02d UTC\n", 
                  gps.time.hour(), gps.time.minute(), gps.time.second());
  }
  
  Serial.println("===========================");
}

// Método privado para leer sensores energéticos usando VoltageReader
void SensorManager::readEnergySensors()
{
  // Leer voltaje real usando VoltageReader
  float voltage = voltageReader.readVoltage();
  
  // Mapear el voltaje del divisor resistivo (2.5V-0V) al voltaje real del sistema
  // Asumiendo que el divisor resistivo mide un rango de 0V a 15V del sistema
  float systemVoltage = voltageReader.readVoltageMapped(VOLTAGE_READER_SYSTEM_VOLTAGE_MIN, VOLTAGE_READER_SYSTEM_VOLTAGE_MAX);
  
  // Simular corriente basada en el voltaje (para demostración)
  // En un sistema real, aquí leerías un sensor de corriente
  float current = systemVoltage / SENSOR_MANAGER_CURRENT_DIVISOR; // Simulación simple
  
  // Convertir float a uint16_t con 2 decimales de precisión
  // Multiplicar por 100 para mantener 2 decimales como enteros
  energyData.volt = (uint16_t)(systemVoltage * 100.0f);
  energyData.amp = (uint16_t)(current * 100.0f);
  
  // Debug: mostrar información de la lectura
  Serial.println("[EnergySensors] Voltaje divisor: " + String(voltage, 3) + "V");
  Serial.printf("[EnergySensors] Voltaje sistema: %d (%.2fV)\n", energyData.volt, systemVoltage);
  Serial.printf("[EnergySensors] Corriente: %d (%.2fA)\n", energyData.amp, current);
}
void SensorManager::update()
{
  unsigned long currentMillis = millis(); // Obtiene el tiempo actual en milisegundos
  while (gpsSerial.available())
  {
    gps.encode(gpsSerial.read());
  }
  saveGpsCoordinatePeriodically();
  if (currentMillis - lastSampleTime >= SAMPLEINTERVALMSATMOSPHERIC || atmosSampleCount == 0)
  {
    lastSampleTime = currentMillis; // Actualiza el tiempo de la última muestra
    readAtmosphericSensors();       // Toma una nueva muestra
  }
}

void SensorManager::verificFullAtmosSamples()
{
  const unsigned long TIMEOUT_MS = 3000; // Tiempo máximo permitido para llenar el buffer (3 segundos)
  unsigned long startTime = millis();
  Protocol::AtmosphericSample errorSample;
  errorSample.temp = SENSOR_ERROR_TEMP;
  errorSample.moisture = SENSOR_ERROR_MOISTURE;
  errorSample.hour = SENSOR_ERROR_TIME_COMPONENT;
  errorSample.minute = SENSOR_ERROR_TIME_COMPONENT;

  while (atmosSampleCount < NUMERO_MUESTRAS_ATMOSFERICAS)
  {
    if (millis() - startTime > TIMEOUT_MS)
    {
      // Timeout alcanzado: llenar todas las posiciones restantes del array con error
      for (int i = atmosSampleCount; i < NUMERO_MUESTRAS_ATMOSFERICAS; ++i)
      {
        atmosSamples[i] = errorSample;
      }
      atmosSampleCount = NUMERO_MUESTRAS_ATMOSFERICAS;
      Serial.println("ADVERTENCIA: Timeout llenando buffer atmosférico. Se completó con errores.");
      break;
    }
    readAtmosphericSensors();
    delay(10);
  }
}
// Método para recolectar datos de los sensores
void SensorManager::readGroundGpsSensors()
{

  // Simulación de lectura de sensores (reemplaza con tu hardware)
  readGroundSensors();
  readEnergySensors();
  readGpsSensors();
}

void SensorManager::readSensorsAtmospheric()
{
  update();                  // llamo  a update por cuestion de actualizacion de muestre
  verificFullAtmosSamples(); // verifico
  atmosSampleCount = 0;      // Reinicia el contador de muestras para limpiar el buffer lógico
}

String SensorManager::getVoltageReaderDebugInfo()
{
  return voltageReader.getDebugInfo();
}

void SensorManager::calibrateVoltageReader(float expectedMin, float expectedMax)
{
  Serial.println("[SensorManager] Iniciando calibración del VoltageReader...");
  voltageReader.calibrate(expectedMin, expectedMax);
  Serial.println("[SensorManager] Calibración del VoltageReader completada.");
}

void SensorManager::saveGpsCoordinatePeriodically() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastGpsSaveTime >= gpsSaveIntervalMs) {
        lastGpsSaveTime = currentMillis;
        if (gps.location.isValid()) {
            gpsHistory[gpsHistoryIndex].latitude = gps.location.lat() * 1e7;
            gpsHistory[gpsHistoryIndex].longitude = gps.location.lng() * 1e7;
            gpsHistory[gpsHistoryIndex].hour = gps.time.isValid() ? gps.time.hour() : 0;
            gpsHistory[gpsHistoryIndex].minute = gps.time.isValid() ? gps.time.minute() : 0;
            gpsHistoryIndex = (gpsHistoryIndex + 1) % GPS_COORDINATE_HISTORY_SIZE;
        }
    }
}

SensorManager::GpsCoordinate SensorManager::getLastGpsCoordinate() const {
    int idx = gpsHistoryIndex == 0 ? GPS_COORDINATE_HISTORY_SIZE - 1 : gpsHistoryIndex - 1;
    return gpsHistory[idx];
}

void SensorManager::setSimulationMode(bool enabled) {
    simulationModeEnabled = enabled;
    DEBUG_PRINTF("SensorManager: Modo simulación %s\n", enabled ? "habilitado" : "deshabilitado");
}

bool SensorManager::isSimulationModeEnabled() const {
    return simulationModeEnabled;
}

String SensorManager::getRS485DebugInfo() {
    String info = "=== RS485 Debug Info ===\n";
    info += "Simulación habilitada: " + String(simulationModeEnabled ? "SÍ" : "NO") + "\n";
    info += "RS485 disponible: " + String(rs485Manager.isAvailable() ? "SÍ" : "NO") + "\n";
    info += "Último error: " + String(rs485Manager.getLastError()) + "\n";
    info += "Intentos fallidos: " + String(rs485Manager.getFailedAttempts()) + "\n";
    return info;
}