
#ifndef SENSOR_UTILS_H
#define SENSOR_UTILS_H

#include "ground_sensor_data.h"

struct RawPotentiometerValues {
  int rawTemp;
  int rawMoisture;
  int rawN;
  int rawP;
  int rawK;
  int rawEC;
  int rawPH;
};

RawPotentiometerValues readRawPotentiometerValues();
void mapRawToGroundSensor(const RawPotentiometerValues& raw, GroundSensor& sensor);
void printGroundSensorData(const GroundSensor& data);
void sendGroundSensorData(const GroundSensor& data);

#endif
