VoltageReader
============

.. automodule:: src.voltage_reader
   :members:
   :undoc-members:
   :show-inheritance:

Descripción
-----------

El **VoltageReader** es una clase especializada para la lectura precisa de voltaje 
desde un divisor resistivo conectado al ADC del ESP32. Proporciona funcionalidades 
avanzadas como filtrado de ruido, calibración automática y mapeo de rangos.

Características Principales
--------------------------

* **Lectura Precisa:** Resolución de 12 bits con atenuación configurable
* **Filtrado de Ruido:** Media móvil para estabilidad de lecturas
* **Calibración Inteligente:** Corrección automática de offset y pendiente
* **Mapeo Flexible:** Conversión a porcentajes o rangos personalizados
* **Validación de Datos:** Verificación de rangos de voltaje esperados

Especificaciones Técnicas
-------------------------

* **Pin ADC:** 34 (configurable)
* **Rango de Entrada:** 0V a 2.5V (divisor resistivo)
* **Resolución:** 12 bits (4096 niveles)
* **Atenuación:** 11dB (0V-2.6V)
* **Filtrado:** Media móvil configurable
* **Calibración:** Offset y pendiente automáticos

Ejemplo de Uso
--------------

.. code-block:: cpp

   #include "voltage_reader.h"
   
   VoltageReader voltageReader;
   
   void setup() {
       voltageReader.begin();
       
       // Opcional: Calibrar el sensor
       voltageReader.calibrate(0.0, 2.5);
   }
   
   void loop() {
       // Leer voltaje directo
       float voltage = voltageReader.readVoltage();
       
       // Leer como porcentaje
       float percentage = voltageReader.readVoltagePercentage();
       
       // Mapear a rango personalizado (0V-15V del sistema)
       float systemVoltage = voltageReader.readVoltageMapped(0.0, 15.0);
       
       // Verificar si la lectura es válida
       if (voltageReader.isReadingValid()) {
           Serial.println("Voltaje: " + String(voltage, 3) + "V");
           Serial.println("Sistema: " + String(systemVoltage, 2) + "V");
       }
   }

Métodos Principales
------------------

begin()
~~~~~~~~

Inicializa el VoltageReader con la configuración del ADC.

.. code-block:: cpp

   void begin();

readVoltage()
~~~~~~~~~~~~

Lee el voltaje actual del divisor resistivo.

.. code-block:: cpp

   float readVoltage();

readVoltagePercentage()
~~~~~~~~~~~~~~~~~~~~~~

Lee el voltaje como porcentaje del rango máximo.

.. code-block:: cpp

   float readVoltagePercentage();

readVoltageMapped()
~~~~~~~~~~~~~~~~~~~

Mapea el voltaje a un rango personalizado.

.. code-block:: cpp

   float readVoltageMapped(float outMin, float outMax);

readRawValue()
~~~~~~~~~~~~~

Lee el valor raw del ADC sin procesamiento.

.. code-block:: cpp

   int readRawValue();

calibrate()
~~~~~~~~~~~

Realiza calibración automática del sensor.

.. code-block:: cpp

   void calibrate(float expectedMin, float expectedMax);

calibrateManual()
~~~~~~~~~~~~~~~~~

Realiza calibración manual con valores específicos.

.. code-block:: cpp

   void calibrateManual(float offset, float slope);

isReadingValid()
~~~~~~~~~~~~~~~

Verifica si la lectura actual está dentro del rango válido.

.. code-block:: cpp

   bool isReadingValid();

getDebugInfo()
~~~~~~~~~~~~~~

Obtiene información detallada del estado del sensor.

.. code-block:: cpp

   String getDebugInfo();

Configuración
-------------

El VoltageReader utiliza las siguientes configuraciones desde `config.h`:

.. code-block:: cpp

   #define VOLTAGE_READER_PIN 34
   #define VOLTAGE_READER_RESOLUTION 12
   #define VOLTAGE_READER_ATTENUATION ADC_ATTEN_DB_11
   #define VOLTAGE_READER_MIN_VOLTAGE 0.0
   #define VOLTAGE_READER_MAX_VOLTAGE 2.5
   #define VOLTAGE_READER_FILTER_SAMPLES 5
   #define VOLTAGE_READER_CALIBRATION_OFFSET 0.0
   #define VOLTAGE_READER_CALIBRATION_SLOPE 1.0

Algoritmo de Filtrado
---------------------

El VoltageReader implementa un filtro de media móvil para reducir el ruido:

.. code-block:: cpp

   // Media móvil simple
   float filteredValue = 0;
   for (int i = 0; i < filterSamples; i++) {
       filteredValue += analogRead(pin);
   }
   filteredValue /= filterSamples;
   
   // Conversión a voltaje
   float voltage = (filteredValue / 4095.0) * maxVoltage;
   
   // Aplicar calibración
   voltage = (voltage - offset) * slope;

Sistema de Calibración
----------------------

**Calibración Automática:**
* Mide múltiples puntos en el rango esperado
* Calcula offset y pendiente automáticamente
* Aplica corrección en tiempo real

**Calibración Manual:**
* Permite ajuste fino de parámetros
* Útil para compensar errores sistemáticos
* Persiste entre reinicios

Validación de Datos
-------------------

* **Rango de Voltaje:** Verifica que esté entre 0V y 2.5V
* **Estabilidad:** Detecta lecturas anómalas
* **Consistencia:** Valida tendencias de cambio
* **Timeout:** Detecta sensores desconectados

Optimizaciones
--------------

* **Lectura Eficiente:** Minimiza tiempo de ADC
* **Memoria Optimizada:** Buffer circular pequeño
* **Energía:** Sleep entre lecturas
* **Precisión:** Calibración automática

Casos de Uso
------------

**Monitoreo de Batería:**
.. code-block:: cpp

   float batteryVoltage = voltageReader.readVoltageMapped(0.0, 15.0);
   if (batteryVoltage < 11.0) {
       // Alerta: Batería baja
   }

**Control de Potencia:**
.. code-block:: cpp

   float powerLevel = voltageReader.readVoltagePercentage();
   if (powerLevel > 80.0) {
       // Reducir consumo
   }

**Calibración Periódica:**
.. code-block:: cpp

   // Calibrar cada 24 horas
   if (millis() - lastCalibration > 86400000) {
       voltageReader.calibrate(0.0, 2.5);
       lastCalibration = millis();
   }

Notas de Implementación
-----------------------

* **Thread-Safe:** Seguro para uso en múltiples contextos
* **Error Handling:** Manejo robusto de fallos
* **Debug Information:** Logging detallado para troubleshooting
* **Configuración Flexible:** Parámetros ajustables

Ver también
----------

* :doc:`sensor_manager`
* :doc:`../user_guide/sensors`
* :doc:`../developer_guide/voltage_measurement` 