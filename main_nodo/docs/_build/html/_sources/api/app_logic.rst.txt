AppLogic
=========

.. automodule:: src.app_logic
   :members:
   :undoc-members:
   :show-inheritance:

Descripción
-----------

El **AppLogic** es el cerebro del sistema de monitoreo agrícola, responsable de 
la lógica de aplicación, toma de decisiones y coordinación entre todos los 
componentes del sistema. Implementa la inteligencia del nodo para optimizar 
el funcionamiento y la transmisión de datos.

Características Principales
--------------------------

* **Coordinación Central:** Orquesta todos los componentes del sistema
* **Lógica de Decisión:** Toma decisiones inteligentes sobre transmisión y operación
* **Gestión de Estado:** Mantiene el estado global del nodo
* **Optimización Energética:** Controla el consumo de energía
* **Gestión de Errores:** Manejo robusto de fallos y recuperación

Arquitectura del Sistema
------------------------

**Componentes Integrados:**
* **SensorManager:** Adquisición de datos sensoriales
* **RadioManager:** Comunicación LoRa mesh
* **NodeIdentity:** Gestión de identidad y configuración
* **VoltageReader:** Monitoreo de energía

**Flujo de Datos:**
.. code-block:: cpp

   SensorManager → AppLogic → RadioManager → Mesh Network
   ↑                                    ↓
   VoltageReader ← AppLogic ← NodeIdentity ← Config

Ejemplo de Uso
--------------

.. code-block:: cpp

   #include "app_logic.h"
   
   AppLogic appLogic;
   
   void setup() {
       appLogic.begin();
   }
   
   void loop() {
       // Ejecutar lógica principal
       appLogic.run();
       
       // Verificar estado del sistema
       if (appLogic.isSystemHealthy()) {
           Serial.println("Sistema funcionando correctamente");
       } else {
           Serial.println("Problemas detectados en el sistema");
       }
   }

Métodos Principales
------------------

begin()
~~~~~~~~

Inicializa la lógica de aplicación y todos los componentes.

.. code-block:: cpp

   void begin();

run()
~~~~~

Ejecuta la lógica principal del sistema en cada iteración.

.. code-block:: cpp

   void run();

processSensorData()
~~~~~~~~~~~~~~~~~~

Procesa los datos de los sensores y toma decisiones.

.. code-block:: cpp

   void processSensorData();

transmitData()
~~~~~~~~~~~~~~

Transmite datos según la estrategia de comunicación.

.. code-block:: cpp

   void transmitData();

isSystemHealthy()
~~~~~~~~~~~~~~~~~

Verifica el estado general del sistema.

.. code-block:: cpp

   bool isSystemHealthy();

getSystemStatus()
~~~~~~~~~~~~~~~~

Obtiene información detallada del estado del sistema.

.. code-block:: cpp

   SystemStatus getSystemStatus();

Estrategias de Transmisión
-------------------------

**Transmisión Inteligente:**
* **Datos Críticos:** Transmisión inmediata
* **Datos Normales:** Transmisión programada
* **Datos Históricos:** Transmisión en lotes
* **Optimización:** Solo transmite cambios significativos

**Priorización de Datos:**
.. code-block:: cpp

   enum DataPriority {
       CRITICAL = 0,    // Alertas y errores
       HIGH = 1,        // Datos importantes
       NORMAL = 2,      // Datos regulares
       LOW = 3          // Datos históricos
   };

Gestión de Energía
------------------

**Estrategias de Ahorro:**
* **Sleep Adaptativo:** Ajusta tiempo de sleep según actividad
* **Transmisión Selectiva:** Solo envía datos necesarios
* **Modo Bajo Consumo:** Reduce frecuencia de muestreo
* **Alerta de Batería:** Notifica cuando la energía es baja

**Estados de Energía:**
.. code-block:: cpp

   enum PowerState {
       NORMAL,           // Operación normal
       CONSERVATION,     // Ahorro de energía
       CRITICAL,         // Batería baja
       EMERGENCY         // Modo de emergencia
   };

Lógica de Decisión
------------------

**Criterios de Transmisión:**
* **Cambio Significativo:** Datos que cambian más del umbral
* **Tiempo Transcurrido:** Transmisión periódica
* **Eventos Críticos:** Alertas y errores
* **Solicitud Externa:** Comandos de control

**Algoritmo de Decisión:**
.. code-block:: cpp

   bool shouldTransmit(SensorData data) {
       // Verificar cambio significativo
       if (abs(data.value - lastValue) > threshold) {
           return true;
       }
       
       // Verificar tiempo transcurrido
       if (millis() - lastTransmission > transmissionInterval) {
           return true;
       }
       
       // Verificar eventos críticos
       if (data.isCritical()) {
           return true;
       }
       
       return false;
   }

Gestión de Errores
------------------

**Tipos de Errores:**
* **Errores de Sensores:** Lecturas fuera de rango
* **Errores de Comunicación:** Fallos en transmisión
* **Errores de Energía:** Batería baja o inestable
* **Errores de Sistema:** Fallos de hardware

**Estrategias de Recuperación:**
* **Reintento Automático:** Reintenta operaciones fallidas
* **Modo Degradado:** Funciona con capacidades limitadas
* **Reset Inteligente:** Reinicio controlado del sistema
* **Logging Detallado:** Registro de errores para análisis

Optimizaciones
--------------

* **Cache Inteligente:** Almacena datos frecuentemente usados
* **Compresión de Datos:** Reduce tamaño de transmisiones
* **Batch Processing:** Procesa datos en lotes
* **Predictive Analysis:** Anticipa necesidades del sistema

Casos de Uso
------------

**Monitoreo Continuo:**
.. code-block:: cpp

   // Ejecutar lógica principal
   appLogic.run();
   
   // Verificar salud del sistema
   if (!appLogic.isSystemHealthy()) {
       // Activar modo de emergencia
       appLogic.activateEmergencyMode();
   }

**Transmisión Inteligente:**
.. code-block:: cpp

   // Procesar datos y decidir transmisión
   appLogic.processSensorData();
   
   // Transmitir solo si es necesario
   if (appLogic.shouldTransmit()) {
       appLogic.transmitData();
   }

**Gestión de Energía:**
.. code-block:: cpp

   // Verificar estado de energía
   PowerState powerState = appLogic.getPowerState();
   
   switch (powerState) {
       case CRITICAL:
           appLogic.activatePowerSavingMode();
           break;
       case EMERGENCY:
           appLogic.activateEmergencyMode();
           break;
   }

Notas de Implementación
-----------------------

* **Thread-Safe:** Seguro para operaciones concurrentes
* **Error Recovery:** Recuperación automática de fallos
* **Debug Logging:** Información detallada de operaciones
* **Configuración Dinámica:** Parámetros ajustables en tiempo real

Ver también
----------

* :doc:`sensor_manager`
* :doc:`radio_manager`
* :doc:`node_identity`
* :doc:`../user_guide/configuration`
* :doc:`../developer_guide/system_architecture` 