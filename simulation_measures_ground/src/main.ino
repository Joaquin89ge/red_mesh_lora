#include "ground_sensor_data.h"
#include "sensor_utils.h"
#include "config.h"
#include "protocol.h"

#ifndef DEBUG_MACROS_H
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#endif

// Variables globales
GroundSensor currentGroundData;

void setup() {
  Serial.begin(115200); // Para debug
  Serial2.begin(BAUD_RATE, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
  
  // Configurar pin DE/RE del MAX485
  pinMode(RS485_DE_PIN, OUTPUT);
  digitalWrite(RS485_DE_PIN, LOW); // Modo recepción inicial
  
  delay(1000);
  DEBUG_PRINTLN("DEBUG: Iniciando simulación de sensor de suelo con potenciómetros.");
  DEBUG_PRINTLN("DEBUG: Modo esclavo RS485 activado.");
  DEBUG_PRINTLN("DEBUG: Protocolo binario activo (sin framing)");
  
  // Diagnóstico de configuración
  DEBUG_PRINTLN("=== DIAGNÓSTICO RS485 ===");
  DEBUG_PRINT("Pin DE/RE: "); DEBUG_PRINTLN(RS485_DE_PIN);
  DEBUG_PRINT("Pin RX: "); DEBUG_PRINTLN(RS485_RX_PIN);
  DEBUG_PRINT("Pin TX: "); DEBUG_PRINTLN(RS485_TX_PIN);
  DEBUG_PRINT("Baud Rate: "); DEBUG_PRINTLN(BAUD_RATE);
  DEBUG_PRINTLN("=========================");
  
  // Tamaño del payload binario esperado
  DEBUG_PRINT("sizeof(GroundSensorBinary): ");
  DEBUG_PRINTLN((int)sizeof(Protocol::GroundSensorBinary));
}


void setRS485Mode(bool transmit) {
  // Asegurar que no hay datos pendientes antes de cambiar modo
  Serial2.flush();
  delay(1);
  
  digitalWrite(RS485_DE_PIN, transmit ? HIGH : LOW);
  
  if (transmit) {
    delay(RS485_TX_DELAY); // Delay para estabilizar
  } else {
    // Delay adicional al volver a modo recepción
    delay(2);
  }
}

// Convertir a payload empaquetado
static inline Protocol::GroundSensorBinary toBinaryPayload(const GroundSensor& src) {
  Protocol::GroundSensorBinary out;
  out.temp     = (int16_t)src.temp;
  out.moisture = (uint16_t)src.moisture;
  out.n        = (uint16_t)src.n;
  out.p        = (uint16_t)src.p;
  out.k        = (uint16_t)src.k;
  out.EC       = (uint16_t)src.EC;
  out.PH       = (uint8_t)src.PH;
  return out;
}

void loop() {
  // Protocolo binario: leer 1 byte de tipo de mensaje
  if (Serial2.available() >= 1) {
    uint8_t msgType = (uint8_t)Serial2.read();
    DEBUG_PRINT("CMD: "); DEBUG_PRINTLN((int)msgType);

    switch (msgType) {
      case Protocol::RS485Type::PING: {
        setRS485Mode(true);
        Serial2.write((uint8_t)Protocol::RS485Type::PONG);
        Serial2.flush();
        delay(RS485_RESPONSE_DELAY);
        setRS485Mode(false);
        DEBUG_PRINTLN("PONG enviado");
        break;
      }
      case Protocol::RS485Type::REQ_GROUND_DATA: {
        // Actualizar datos desde potenciómetros
        RawPotentiometerValues rawValues = readRawPotentiometerValues();
        mapRawToGroundSensor(rawValues, currentGroundData);

        Protocol::GroundSensorBinary payload = toBinaryPayload(currentGroundData);
        setRS485Mode(true);
        Serial2.write((uint8_t)Protocol::RS485Type::DATA_GROUND);
        Serial2.write((const uint8_t*)&payload, sizeof(payload));
        Serial2.flush();
        delay(RS485_RESPONSE_DELAY);
        setRS485Mode(false);

        DEBUG_PRINT("TX bytes: "); DEBUG_PRINTLN(1 + (int)sizeof(payload));
        break;
      }
      default:
        // Opcional: responder error o ignorar
        DEBUG_PRINT("CMD desconocido: "); DEBUG_PRINTLN((int)msgType);
        break;
    }
  }
  else{
    //DEBUG_PRINT("no availeble serial");
  }
  
  // Pequeño delay para estabilidad
  delay(2);
}
