
#include "sensor_utils.h"
#include "ground_sensor_data.h"
#include "config.h"
#include <Arduino.h>

#ifndef DEBUG_MACROS_H
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#endif

RawPotentiometerValues readRawPotentiometerValues() {
  RawPotentiometerValues raw;
  raw.rawTemp = analogRead(36);
  raw.rawMoisture = analogRead(39);
  raw.rawN = analogRead(32);
  raw.rawP = analogRead(33);
  raw.rawK = analogRead(27);
  raw.rawEC = analogRead(34);
  raw.rawPH = analogRead(35);

  DEBUG_PRINTLN("--- Lectura de Potenciometros (Valores Brutos) ---");
  DEBUG_PRINT("Temp Raw: "); DEBUG_PRINTLN(raw.rawTemp);
  DEBUG_PRINT("Moisture Raw: "); DEBUG_PRINTLN(raw.rawMoisture);
  DEBUG_PRINT("N Raw: "); DEBUG_PRINTLN(raw.rawN);
  DEBUG_PRINT("P Raw: "); DEBUG_PRINTLN(raw.rawP);
  DEBUG_PRINT("K Raw: "); DEBUG_PRINTLN(raw.rawK);
  DEBUG_PRINT("EC Raw: "); DEBUG_PRINTLN(raw.rawEC);
  DEBUG_PRINT("PH Raw: "); DEBUG_PRINTLN(raw.rawPH);
  return raw;
}

void mapRawToGroundSensor(const RawPotentiometerValues& rawValues, GroundSensor& groundData) {
  // Mapeo actualizado para rangos más realistas según el protocolo RS485
  groundData.temp = map(rawValues.rawTemp, POT_MIN, POT_MAX, TEMP_MIN * 10, TEMP_MAX * 10);
  groundData.moisture = map(rawValues.rawMoisture, POT_MIN, POT_MAX, HUMIDITY_MIN * 10, HUMIDITY_MAX * 10);
  groundData.n = map(rawValues.rawN, POT_MIN, POT_MAX, NPK_MIN, NPK_MAX);
  groundData.p = map(rawValues.rawP, POT_MIN, POT_MAX, NPK_MIN, NPK_MAX);
  groundData.k = map(rawValues.rawK, POT_MIN, POT_MAX, NPK_MIN, NPK_MAX);
  groundData.EC = map(rawValues.rawEC, POT_MIN, POT_MAX, EC_MIN, EC_MAX * 10);
  groundData.PH = map(rawValues.rawPH, POT_MIN, POT_MAX, PH_MIN * 10, PH_MAX * 10);
}

void printGroundSensorData(const GroundSensor& groundData) {
  DEBUG_PRINTLN("--- Valores de GroundSensor Mapeados ---");
  DEBUG_PRINT("Temp: "); DEBUG_PRINT((float)groundData.temp / 10.0); DEBUG_PRINTLN(" °C");
  DEBUG_PRINT("Moisture: "); DEBUG_PRINT((float)groundData.moisture / 10.0); DEBUG_PRINTLN(" %RH");
  DEBUG_PRINT("N: "); DEBUG_PRINT(groundData.n); DEBUG_PRINTLN(" ppm");
  DEBUG_PRINT("P: "); DEBUG_PRINT(groundData.p); DEBUG_PRINTLN(" ppm");
  DEBUG_PRINT("K: "); DEBUG_PRINT(groundData.k); DEBUG_PRINTLN(" ppm");
  DEBUG_PRINT("EC: "); DEBUG_PRINT(groundData.EC); DEBUG_PRINTLN(" μS/cm");
  DEBUG_PRINT("PH: "); DEBUG_PRINT((float)groundData.PH / 10.0); DEBUG_PRINTLN("");
  DEBUG_PRINTLN("------------------------------------");
}

void sendGroundSensorData(const GroundSensor& groundData) {
  Serial2.write((const byte*)&groundData, sizeof(groundData));
  DEBUG_PRINTLN("DEBUG: Estructura GroundSensor enviada por Serial.");
}
