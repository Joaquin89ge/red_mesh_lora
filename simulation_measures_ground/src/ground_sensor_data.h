#ifndef GROUND_SENSOR_DATA_H
#define GROUND_SENSOR_DATA_H

#include <Arduino.h>

// Definiciones de salida en décimas
#define TEMP_OUTPUT_MIN     -400  // -40.0 °C
#define TEMP_OUTPUT_MAX     800   // 80.0 °C

#define MOISTURE_OUTPUT_MIN 0     // 0.0 %
#define MOISTURE_OUTPUT_MAX 1000  // 100.0 %

#define NPK_OUTPUT_MIN      1     // mg/kg o mg/L
#define NPK_OUTPUT_MAX      1999

#define EC_OUTPUT_MIN       0     // µS/cm
#define EC_OUTPUT_MAX       20000

#define PH_OUTPUT_MIN       30    // 3.0
#define PH_OUTPUT_MAX       90    // 9.0

// Estructura para datos del sensor
struct GroundSensor {
  int temp;        // Temperatura en décimas de grado Celsius
  int moisture;    // Humedad en décimas de %
  int n;           // Nitrógeno en mg/kg
  int p;           // Fósforo en mg/L
  int k;           // Potasio en mg/L
  int EC;          // Conductividad en µS/cm
  int PH;          // pH en décimas
};

#endif 