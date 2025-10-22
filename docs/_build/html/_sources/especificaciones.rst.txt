Especificaciones Técnicas
==========================

A continuación se detallan las principales especificaciones técnicas del sistema Red Mesh Agrícola.

.. list-table:: Hardware del Gateway
   :header-rows: 1

   * - Componente
     - Especificación
     - Función
   * - Microcontrolador
     - ESP8266 80MHz
     - Procesamiento y control
   * - Radio
     - SX1278 LoRa
     - Comunicación mesh
   * - RTC
     - DS1302 3-wire
     - Sincronización temporal
   * - Almacenamiento
     - LittleFS 4MB
     - Persistencia de datos
   * - Memoria RAM
     - 80KB
     - Buffer de operación

.. list-table:: Comunicación LoRa
   :header-rows: 1

   * - Parámetro
     - Valor
   * - Frecuencia
     - 433 MHz (configurable)
   * - Potencia
     - Hasta 20 dBm
   * - Modulación
     - LoRa (SF7-SF12)
   * - Ancho de Banda
     - 125 kHz
   * - Tasa de Datos
     - 0.3-37.5 kbps
   * - Alcance
     - Hasta 10km en condiciones óptimas

.. note::
   El sistema está diseñado para ser escalable y eficiente, soportando hasta 250 nodos y múltiples tipos de sensores. 