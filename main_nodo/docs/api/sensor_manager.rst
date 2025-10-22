SensorManager
============

.. automodule:: src.sensor_manager
   :members:
   :undoc-members:
   :show-inheritance:

Descripción
-----------

El **SensorManager** es la clase central responsable de la gestión de todos los sensores 
del sistema de monitoreo agrícola. Proporciona una interfaz unificada para la adquisición, 
procesamiento y almacenamiento de datos sensoriales.

Características Principales
--------------------------

* **Gestión Centralizada:** Control unificado de todos los sensores
* **Buffer Circular:** Almacenamiento eficiente de muestras atmosféricas
* **Integración GPS:** Asociación automática de coordenadas y tiempo
* **VoltageReader Integrado:** Lectura precisa de voltaje del sistema
* **Validación de Datos:** Verificación de rangos y detección de errores

Componentes Integrados
---------------------

* **DHT Sensor:** Temperatura y humedad ambiental
* **GPS Module:** Geolocalización y sincronización temporal
* **VoltageReader:** Monitoreo de energía del sistema
* **Sensores de Suelo:** NPK, pH, EC, temperatura y humedad

Ejemplo de Uso
--------------

.. code-block:: cpp

   #include "sensor_manager.h"
   
   SensorManager sensorManager;
   
   void setup() {
       sensorManager.begin();
   }
   
   void loop() {
       // Actualizar sensores atmosféricos
       sensorManager.update();
       
       // Leer todos los sensores
       sensorManager.readGroundGpsSensors();
       
       // Obtener información de debug
       String debugInfo = sensorManager.getVoltageReaderDebugInfo();
       Serial.println(debugInfo);
   }

Métodos Principales
------------------

begin()
~~~~~~~~

Inicializa todos los sensores y la comunicación.

.. code-block:: cpp

   void begin();

update()
~~~~~~~~

Actualiza las lecturas de sensores atmosféricos según el intervalo configurado.

.. code-block:: cpp

   void update();

readGroundGpsSensors()
~~~~~~~~~~~~~~~~~~~~~~

Lee y almacena datos de sensores de suelo, energía y GPS.

.. code-block:: cpp

   void readGroundGpsSensors();

readSensorsAtmospheric()
~~~~~~~~~~~~~~~~~~~~~~~~

Prepara el buffer de muestras atmosféricas para transmisión.

.. code-block:: cpp

   void readSensorsAtmospheric();

getVoltageReaderDebugInfo()
~~~~~~~~~~~~~~~~~~~~~~~~~~

Obtiene información detallada del estado del VoltageReader.

.. code-block:: cpp

   String getVoltageReaderDebugInfo();

calibrateVoltageReader()
~~~~~~~~~~~~~~~~~~~~~~~~

Realiza calibración del VoltageReader.

.. code-block:: cpp

   void calibrateVoltageReader(float expectedMin, float expectedMax);

Estructuras de Datos
--------------------

atmosSamples[]
~~~~~~~~~~~~~~

Buffer circular de muestras atmosféricas.

.. code-block:: cpp

   Protocol::AtmosphericSample atmosSamples[NUMERO_MUESTRAS_ATMOSFERICAS];

groundData
~~~~~~~~~~

Última muestra de sensores de suelo.

.. code-block:: cpp

   Protocol::GroundSensor groundData;

gpsData
~~~~~~~

Última muestra de datos GPS.

.. code-block:: cpp

   Protocol::GpsSensor gpsData;

energyData
~~~~~~~~~~

Última muestra de datos energéticos.

.. code-block:: cpp

   Protocol::EnergyData energyData;

Configuración
-------------

El SensorManager utiliza las siguientes configuraciones desde `config.h`:

* **SAMPLEINTERVAL:** Intervalo de muestreo en milisegundos
* **NUMERO_MUESTRAS_ATMOSFERICAS:** Tamaño del buffer circular
* **PIN_SENS_DHTT:** Pin del sensor DHT
* **GPS_RX_UART2:** Pin RX del GPS (UART2 HardwareSerial)
* **GPS_TX_UART2:** Pin TX del GPS (UART2 HardwareSerial)

Optimizaciones
--------------

* **Sleep Inteligente:** Reduce consumo entre lecturas
* **Validación de Rangos:** Detecta errores de sensores
* **Buffer Eficiente:** Evita desbordamiento de memoria
* **Transmisión Selectiva:** Solo envía datos críticos

Notas de Implementación
-----------------------

* La clase sigue el principio de **Single Responsibility**
* Utiliza **Dependency Injection** para VoltageReader
* Implementa **Error Handling** robusto
* Proporciona **Debug Information** detallada

Ver también
----------

* :doc:`voltage_reader`
* :doc:`protocol`
* :doc:`../user_guide/sensors` 