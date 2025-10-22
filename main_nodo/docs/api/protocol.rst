.. _protocol:

Protocolo de Comunicación
========================

.. doxygenfile:: protocol.h
   :project: main_nodo

.. note::
   **Actualización:**
   El paquete ``GroundGpsPacket`` ahora incluye un campo ``energy`` de tipo ``EnergyData`` para enviar datos energéticos junto con los datos de suelo y GPS.

   .. code-block:: cpp

      struct GroundGpsPacket {
          GroundSensor ground; // Datos de suelo
          GpsSensor gps;       // Datos GPS
          EnergyData energy;   // Datos energéticos (voltaje y corriente)
      };

   Esto permite que el nodo envíe en un solo mensaje los datos de suelo, GPS y energía al gateway.

Protocol
========

.. automodule:: src.protocol
   :members:
   :undoc-members:
   :show-inheritance:

Descripción
-----------

El **Protocol** define las estructuras de datos y constantes utilizadas 
en todo el sistema de monitoreo agrícola. Proporciona una interfaz 
unificada para el intercambio de datos entre componentes y la transmisión 
por la red mesh LoRa.

Características Principales
--------------------------

* **Estructuras Unificadas:** Definiciones consistentes de datos
* **Constantes del Sistema:** Valores de configuración centralizados
* **Tipos de Datos Optimizados:** Uso eficiente de memoria
* **Validación de Datos:** Verificación de integridad
* **Compatibilidad:** Soporte para diferentes versiones del protocolo

Estructuras de Datos
--------------------

AtmosphericSample
~~~~~~~~~~~~~~~~~

Estructura para almacenar muestras atmosféricas.

.. code-block:: cpp

   struct AtmosphericSample {
       int16_t temp;        // Temperatura (x10 para precisión)
       uint16_t moisture;   // Humedad (x10 para precisión)
       uint8_t hour;        // Hora de la muestra
       uint8_t minute;      // Minuto de la muestra
   };

GroundSensor
~~~~~~~~~~~

Estructura para datos de sensores de suelo.

.. code-block:: cpp

   struct GroundSensor {
       int16_t temp;        // Temperatura del suelo
       uint16_t moisture;   // Humedad del suelo
       uint16_t n;          // Nitrógeno (mg/kg)
       uint16_t p;          // Fósforo (mg/kg)
       uint16_t k;          // Potasio (mg/kg)
       uint16_t EC;         // Conductividad eléctrica
       uint16_t PH;         // pH (x10 para precisión)
   };

GpsSensor
~~~~~~~~~

Estructura para datos GPS.

.. code-block:: cpp

   struct GpsSensor {
       float latitude;       // Latitud
       float longitude;      // Longitud
       float altitude;       // Altitud
       uint8_t hour;         // Hora UTC
       uint8_t minute;       // Minuto UTC
       uint8_t flags;        // Flags de validación
   };

EnergyData
~~~~~~~~~~

Estructura para datos energéticos.

.. code-block:: cpp

   struct EnergyData {
       float volt;           // Voltaje del sistema
       float amp;            // Corriente del sistema
   };

Constantes del Sistema
---------------------

Valores de Error
~~~~~~~~~~~~~~~

.. code-block:: cpp

   #define SENSOR_ERROR_TEMP -999
   #define SENSOR_ERROR_MOISTURE 999
   #define SENSOR_ERROR_TIME_COMPONENT 99
   #define SENSOR_ERROR_LATITUDE -999.0
   #define SENSOR_ERROR_LONGITUDE -999.0
   #define SENSOR_ERROR_ALTITUDE -999.0

Configuración de Sensores
~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   #define NUMERO_MUESTRAS_ATMOSFERICAS 10
   #define SAMPLEINTERVAL 2000  // 2 segundos

Flags de Validación GPS
~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   #define GPS_LOCATION_VALID 0x01
   #define GPS_ALTITUDE_VALID 0x02
   #define GPS_TIME_VALID 0x04

Ejemplo de Uso
--------------

.. code-block:: cpp

   #include "protocol.h"
   
   // Crear muestra atmosférica
   Protocol::AtmosphericSample sample;
   sample.temp = 250;        // 25.0°C
   sample.moisture = 650;    // 65.0%
   sample.hour = 14;
   sample.minute = 30;
   
   // Crear datos de suelo
   Protocol::GroundSensor ground;
   ground.temp = -201;       // -20.1°C
   ground.moisture = 750;    // 75.0%
   ground.n = 1000;          // 1000 mg/kg
   ground.p = 150;           // 150 mg/kg
   ground.k = 133;           // 133 mg/kg
   ground.EC = 2556;         // 2556 μS/cm
   ground.PH = 68;           // 6.8 pH
   
   // Crear datos GPS
   Protocol::GpsSensor gps;
   gps.latitude = -34.6037;
   gps.longitude = -58.3816;
   gps.altitude = 25.0;
   gps.hour = 14;
   gps.minute = 30;
   gps.flags = GPS_LOCATION_VALID | GPS_TIME_VALID;

Validación de Datos
-------------------

**Validación de Temperatura:**
.. code-block:: cpp

   bool isValidTemperature(int16_t temp) {
       return temp != SENSOR_ERROR_TEMP && 
              temp >= -400 && temp <= 800;  // -40°C a 80°C
   }

**Validación de Humedad:**
.. code-block:: cpp

   bool isValidMoisture(uint16_t moisture) {
       return moisture != SENSOR_ERROR_MOISTURE && 
              moisture <= 1000;  // 0% a 100%
   }

**Validación de GPS:**
.. code-block:: cpp

   bool isValidGpsLocation(const GpsSensor& gps) {
       return (gps.flags & GPS_LOCATION_VALID) &&
              gps.latitude != SENSOR_ERROR_LATITUDE &&
              gps.longitude != SENSOR_ERROR_LONGITUDE;
   }

Serialización de Datos
---------------------

**Conversión a Bytes:**
.. code-block:: cpp

   void serializeAtmosphericSample(const AtmosphericSample& sample, 
                                  uint8_t* buffer) {
       buffer[0] = (sample.temp >> 8) & 0xFF;
       buffer[1] = sample.temp & 0xFF;
       buffer[2] = (sample.moisture >> 8) & 0xFF;
       buffer[3] = sample.moisture & 0xFF;
       buffer[4] = sample.hour;
       buffer[5] = sample.minute;
   }

**Conversión desde Bytes:**
.. code-block:: cpp

   void deserializeAtmosphericSample(const uint8_t* buffer, 
                                    AtmosphericSample& sample) {
       sample.temp = (buffer[0] << 8) | buffer[1];
       sample.moisture = (buffer[2] << 8) | buffer[3];
       sample.hour = buffer[4];
       sample.minute = buffer[5];
   }

Optimizaciones de Memoria
-------------------------

**Tamaños Optimizados:**
* **Temperatura:** int16_t (-32768 a 32767, precisión 0.1°C)
* **Humedad:** uint16_t (0 a 65535, precisión 0.1%)
* **Coordenadas:** float (precisión de 7 decimales)
* **Flags:** uint8_t (8 bits para múltiples flags)

**Alineación de Datos:**
.. code-block:: cpp

   #pragma pack(push, 1)  // Alineación de 1 byte
   struct OptimizedPacket {
       uint8_t header;
       uint8_t nodeId;
       AtmosphericSample atmos;
       GroundSensor ground;
       GpsSensor gps;
       EnergyData energy;
   };
   #pragma pack(pop)

Compatibilidad de Versiones
--------------------------

**Versión del Protocolo:**
.. code-block:: cpp

   #define PROTOCOL_VERSION 1
   #define PROTOCOL_SUBVERSION 0

**Estructura de Header:**
.. code-block:: cpp

   struct ProtocolHeader {
       uint8_t version;
       uint8_t subversion;
       uint8_t packetType;
       uint8_t nodeId;
       uint32_t timestamp;
   };

Casos de Uso
------------

**Transmisión de Datos:**
.. code-block:: cpp

   // Preparar paquete de datos
   Protocol::AtmosphericSample samples[10];
   // ... llenar datos ...
   
   // Serializar para transmisión
   uint8_t buffer[256];
   serializeData(samples, buffer, sizeof(samples));

**Validación de Datos Recibidos:**
.. code-block:: cpp

   // Deserializar datos recibidos
   Protocol::GroundSensor groundData;
   deserializeGroundData(receivedBuffer, groundData);
   
   // Validar datos
   if (isValidGroundData(groundData)) {
       processGroundData(groundData);
   }

**Debug de Datos:**
.. code-block:: cpp

   void printAtmosphericSample(const AtmosphericSample& sample) {
       Serial.print("Temp: "); Serial.print(sample.temp / 10.0);
       Serial.print("°C, Hum: "); Serial.print(sample.moisture / 10.0);
       Serial.print("%, Time: "); Serial.print(sample.hour);
       Serial.print(":"); Serial.println(sample.minute);
   }

Notas de Implementación
-----------------------

* **Endianness:** Los datos se transmiten en little-endian
* **Checksum:** Validación de integridad en transmisiones
* **Compresión:** Datos comprimidos para ahorrar ancho de banda
* **Versionado:** Soporte para múltiples versiones del protocolo

Ver también
----------

* :doc:`sensor_manager`
* :doc:`radio_manager`
* :doc:`../user_guide/data_format`
* :doc:`../developer_guide/protocol_design` 