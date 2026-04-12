RadioManager
============

.. automodule:: src.radio_manager
   :members:
   :undoc-members:
   :show-inheritance:

Descripción
-----------

El **RadioManager** es responsable de la gestión de la comunicación LoRa mesh 
en el sistema de monitoreo agrícola. Proporciona funcionalidades avanzadas 
para transmisión, recepción y enrutamiento de datos en la red mesh.

Características Principales
--------------------------

* **Comunicación LoRa:** Transmisión de largo alcance (hasta 10km)
* **Red Mesh:** Enrutamiento dinámico y redundante
* **Protocolo Personalizado:** Optimizado para datos agrícolas
* **Gestión de Congestión:** Control de tráfico de red
* **Retransmisión Automática:** Garantía de entrega de datos

Especificaciones Técnicas
-------------------------

* **Frecuencia:** 433 MHz (configurable)
* **Potencia:** Hasta 20 dBm
* **Modulación:** LoRa (SF7-SF12)
* **Ancho de Banda:** 125 kHz
* **Tasa de Datos:** 0.3-37.5 kbps
* **Alcance:** 10km en condiciones óptimas

Ejemplo de Uso
--------------

.. code-block:: cpp

   #include "radio_manager.h"
   
   RadioManager radioManager;
   
   void setup() {
       radioManager.begin();
       radioManager.setNodeId(1);
   }
   
   void loop() {
       // Enviar datos
       if (radioManager.sendData(data, size)) {
           Serial.println("Datos enviados exitosamente");
       }
       
       // Procesar datos recibidos
       if (radioManager.dataAvailable()) {
           uint8_t receivedData[256];
           uint8_t size = radioManager.receiveData(receivedData);
           processReceivedData(receivedData, size);
       }
   }

Métodos Principales
------------------

begin()
~~~~~~~~

Inicializa el módulo LoRa y configura los parámetros de comunicación.

.. code-block:: cpp

   void begin();

setNodeId()
~~~~~~~~~~~

Configura el ID único del nodo en la red mesh.

.. code-block:: cpp

   void setNodeId(uint8_t nodeId);

sendData()
~~~~~~~~~~

Envía datos a través de la red LoRa mesh.

.. code-block:: cpp

   bool sendData(uint8_t* data, uint8_t size);

receiveData()
~~~~~~~~~~~~

Recibe datos de la red LoRa mesh.

.. code-block:: cpp

   uint8_t receiveData(uint8_t* buffer);

dataAvailable()
~~~~~~~~~~~~~~

Verifica si hay datos disponibles para recibir.

.. code-block:: cpp

   bool dataAvailable();

getNetworkStatus()
~~~~~~~~~~~~~~~~~~

Obtiene el estado actual de la red mesh.

.. code-block:: cpp

   NetworkStatus getNetworkStatus();

Configuración de Red
--------------------

El RadioManager utiliza las siguientes configuraciones desde `config.h`:

.. code-block:: cpp

   #define LORA_FREQUENCY 433E6
   #define LORA_POWER 20
   #define LORA_SPREADING_FACTOR 7
   #define LORA_SIGNAL_BANDWIDTH 125E3
   #define LORA_CODING_RATE 5
   #define LORA_PREAMBLE_LENGTH 8
   #define LORA_SYNC_WORD 0x12

Protocolo de Comunicación
-------------------------

**Estructura de Paquete:**
.. code-block:: cpp

   struct LoRaPacket {
       uint8_t header;        // 0xAA
       uint8_t nodeId;        // ID del nodo
       uint8_t packetType;    // Tipo de datos
       uint8_t sequence;      // Número de secuencia
       uint8_t data[240];     // Datos del paquete
       uint8_t checksum;      // Checksum para validación
   };

**Tipos de Paquetes:**
* **0x01:** Datos atmosféricos
* **0x02:** Datos de suelo
* **0x03:** Datos GPS
* **0x04:** Datos de energía
* **0x05:** Comando de control
* **0x06:** Respuesta de estado

Enrutamiento Mesh
-----------------

**Algoritmo de Enrutamiento:**
* **Flooding Controlado:** Transmisión con TTL
* **Rutas Múltiples:** Redundancia de comunicación
* **Detección de Vecinos:** Descubrimiento automático
* **Optimización de Rutas:** Selección de mejor camino

**TTL (Time To Live):**
.. code-block:: cpp

   #define MESH_TTL_DEFAULT 5
   #define MESH_TTL_MAX 10
   #define MESH_TTL_CRITICAL 15

Gestión de Congestión
---------------------

* **Control de Flujo:** Limita transmisiones por nodo
* **Priorización:** Datos críticos tienen prioridad
* **Backoff Exponencial:** Reduce colisiones
* **Monitoreo de Calidad:** RSSI y SNR

Optimizaciones
--------------

* **Sleep Inteligente:** Reduce consumo entre transmisiones
* **Compresión de Datos:** Minimiza tamaño de paquetes
* **Ack Selectivo:** Solo confirma paquetes críticos
* **Cache de Rutas:** Evita recálculo frecuente

Casos de Uso
------------

**Transmisión de Datos Sensoriales:**
.. code-block:: cpp

   // Enviar datos atmosféricos
   uint8_t data[] = {temp, humidity, pressure};
   radioManager.sendData(data, sizeof(data));

**Comunicación de Control:**
.. code-block:: cpp

   // Enviar comando de calibración
   uint8_t command[] = {0x05, 0x01, 0x00};
   radioManager.sendData(command, sizeof(command));

**Monitoreo de Red:**
.. code-block:: cpp

   NetworkStatus status = radioManager.getNetworkStatus();
   if (status.connectedNodes < 3) {
       // Alerta: Red poco conectada
   }

Notas de Implementación
-----------------------

* **Thread-Safe:** Seguro para uso concurrente
* **Error Recovery:** Recuperación automática de fallos
* **Debug Logging:** Información detallada de comunicación
* **Configuración Dinámica:** Parámetros ajustables en tiempo real

Ver también
----------

* :doc:`sensor_manager`
* :doc:`protocol`
* :doc:`../user_guide/communication`
* :doc:`../developer_guide/mesh_networking` 