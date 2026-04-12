NodeIdentity
============

.. automodule:: src.node_identity
   :members:
   :undoc-members:
   :show-inheritance:

Descripción
-----------

El **NodeIdentity** es responsable de la gestión de la identidad única del nodo 
en la red mesh agrícola. Proporciona funcionalidades para configuración, 
identificación y gestión de parámetros específicos de cada nodo.

Características Principales
--------------------------

* **Identidad Única:** Gestión del ID único del nodo
* **Configuración Persistente:** Almacenamiento en EEPROM/SPIFFS
* **Parámetros de Red:** Configuración de comunicación mesh
* **Información de Ubicación:** Datos GPS y descripción del sitio
* **Gestión de Versiones:** Control de versiones de firmware

Especificaciones Técnicas
-------------------------

* **Almacenamiento:** EEPROM/SPIFFS para persistencia
* **ID Único:** 8 bits (0-255 nodos por red)
* **Configuración:** Parámetros personalizables por nodo
* **Validación:** Verificación de integridad de datos
* **Backup:** Respaldo automático de configuración

Ejemplo de Uso
--------------

.. code-block:: cpp

   #include "node_identity.h"
   
   NodeIdentity nodeIdentity;
   
   void setup() {
       nodeIdentity.begin();
       
       // Configurar ID del nodo
       nodeIdentity.setNodeId(1);
       
       // Configurar parámetros de red
       nodeIdentity.setNetworkParameters(433E6, 20, 7);
   }
   
   void loop() {
       // Obtener información del nodo
       uint8_t nodeId = nodeIdentity.getNodeId();
       String location = nodeIdentity.getLocation();
       
       Serial.println("Nodo ID: " + String(nodeId));
       Serial.println("Ubicación: " + location);
   }

Métodos Principales
------------------

begin()
~~~~~~~~

Inicializa el NodeIdentity y carga la configuración guardada.

.. code-block:: cpp

   void begin();

setNodeId()
~~~~~~~~~~~

Configura el ID único del nodo en la red.

.. code-block:: cpp

   void setNodeId(uint8_t nodeId);

getNodeId()
~~~~~~~~~~~

Obtiene el ID actual del nodo.

.. code-block:: cpp

   uint8_t getNodeId();

setLocation()
~~~~~~~~~~~~~

Configura la ubicación geográfica del nodo.

.. code-block:: cpp

   void setLocation(const String& location);

getLocation()
~~~~~~~~~~~~~

Obtiene la ubicación configurada del nodo.

.. code-block:: cpp

   String getLocation();

setNetworkParameters()
~~~~~~~~~~~~~~~~~~~~~

Configura los parámetros de comunicación de la red.

.. code-block:: cpp

   void setNetworkParameters(long frequency, int power, int spreadingFactor);

saveConfiguration()
~~~~~~~~~~~~~~~~~~

Guarda la configuración actual en memoria persistente.

.. code-block:: cpp

   bool saveConfiguration();

loadConfiguration()
~~~~~~~~~~~~~~~~~~

Carga la configuración desde memoria persistente.

.. code-block:: cpp

   bool loadConfiguration();

getNodeInfo()
~~~~~~~~~~~~~

Obtiene información completa del nodo.

.. code-block:: cpp

   NodeInfo getNodeInfo();

Estructura de Configuración
---------------------------

**NodeInfo:**
.. code-block:: cpp

   struct NodeInfo {
       uint8_t nodeId;           // ID único del nodo
       String location;          // Ubicación geográfica
       String description;       // Descripción del sitio
       long frequency;           // Frecuencia LoRa
       int power;               // Potencia de transmisión
       int spreadingFactor;      // Factor de dispersión
       String firmwareVersion;   // Versión del firmware
       unsigned long uptime;     // Tiempo de funcionamiento
   };

**Configuración de Red:**
.. code-block:: cpp

   struct NetworkConfig {
       uint8_t nodeId;
       long frequency;
       int power;
       int spreadingFactor;
       int bandwidth;
       int codingRate;
       int preambleLength;
   };

Persistencia de Datos
---------------------

**Almacenamiento en EEPROM:**
.. code-block:: cpp

   #define EEPROM_NODE_ID_ADDR 0
   #define EEPROM_LOCATION_ADDR 1
   #define EEPROM_FREQUENCY_ADDR 32
   #define EEPROM_POWER_ADDR 36
   #define EEPROM_SPREADING_FACTOR_ADDR 40

**Validación de Integridad:**
.. code-block:: cpp

   bool validateConfiguration() {
       // Verificar checksum
       uint16_t storedChecksum = EEPROM.read(EEPROM_CHECKSUM_ADDR);
       uint16_t calculatedChecksum = calculateChecksum();
       
       return storedChecksum == calculatedChecksum;
   }

Gestión de Versiones
-------------------

**Información de Versión:**
.. code-block:: cpp

   struct VersionInfo {
       uint8_t major;
       uint8_t minor;
       uint8_t patch;
       String buildDate;
       String buildTime;
   };

**Actualización de Firmware:**
* **Verificación:** Valida integridad del nuevo firmware
* **Backup:** Guarda versión anterior
* **Rollback:** Permite volver a versión anterior
* **Notificación:** Informa sobre actualizaciones

Configuración por Defecto
-------------------------

**Valores Iniciales:**
.. code-block:: cpp

   #define DEFAULT_NODE_ID 0
   #define DEFAULT_FREQUENCY 433E6
   #define DEFAULT_POWER 20
   #define DEFAULT_SPREADING_FACTOR 7
   #define DEFAULT_LOCATION "Sin configurar"
   #define DEFAULT_DESCRIPTION "Nodo agrícola"

**Configuración Automática:**
* **Auto-Detección:** Identifica tipo de sensor automáticamente
* **Configuración por GPS:** Usa coordenadas para configuración
* **Aprendizaje:** Adapta parámetros según uso

Casos de Uso
------------

**Configuración Inicial:**
.. code-block:: cpp

   // Configurar nodo por primera vez
   nodeIdentity.begin();
   nodeIdentity.setNodeId(1);
   nodeIdentity.setLocation("Campo Norte - Sector A");
   nodeIdentity.saveConfiguration();

**Actualización de Configuración:**
.. code-block:: cpp

   // Cambiar parámetros de red
   nodeIdentity.setNetworkParameters(868E6, 18, 8);
   nodeIdentity.saveConfiguration();

**Información del Sistema:**
.. code-block:: cpp

   // Obtener información completa
   NodeInfo info = nodeIdentity.getNodeInfo();
   Serial.println("Nodo: " + String(info.nodeId));
   Serial.println("Ubicación: " + info.location);
   Serial.println("Firmware: " + info.firmwareVersion);

Notas de Implementación
-----------------------

* **Thread-Safe:** Seguro para operaciones concurrentes
* **Error Recovery:** Recuperación automática de fallos
* **Validación:** Verificación de integridad de datos
* **Backup:** Respaldo automático de configuración

Ver también
----------

* :doc:`app_logic`
* :doc:`radio_manager`
* :doc:`../user_guide/configuration`
* :doc:`../developer_guide/node_setup` 