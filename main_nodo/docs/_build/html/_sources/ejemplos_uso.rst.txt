Ejemplos de Uso
===============

Esta sección muestra ejemplos prácticos para desplegar, configurar y utilizar el sistema de monitoreo agrícola.

Configuración Inicial
---------------------
.. code-block:: bash

   # Clonar el repositorio
   git clone https://github.com/tu_usuario/sistema-monitoreo-agricola.git
   cd sistema-monitoreo-agricola

   # Compilar el firmware
   platformio run

   # Subir el firmware al nodo
   platformio upload

Lectura de Sensores
-------------------
.. code-block:: cpp

   // Ejemplo de lectura de temperatura y humedad
   float temp = sensorManager.readTemperature();
   float hum = sensorManager.readHumidity();

Envío de Datos
--------------
.. code-block:: cpp

   // Enviar datos al gateway
   appLogic.sendAtmosphericData();
   appLogic.sendGroungGpsData();

Integración con Gateway
----------------------
.. code-block:: json

   {
     "ground": {"temp": 210, "moisture": 650, ...},
     "gps": {"latitude": -345678901, "longitude": -584321234, ...},
     "energy": {"volt": 12.5, "amp": 0.13}
   }

.. note::
   Consulta la sección de API para detalles sobre los mensajes y estructuras de datos. 