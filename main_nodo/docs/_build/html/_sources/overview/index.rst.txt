Visión General del Sistema
==========================

Introducción
------------
El Sistema de Monitoreo Agrícola con Red Mesh LoRa es una solución integral para la agricultura de precisión, combinando tecnologías IoT avanzadas con arquitectura de software robusta y escalable.

Características Principales
--------------------------
- **Red Mesh LoRa:** Comunicación de largo alcance (hasta 10km)
- **Arquitectura Modular:** Principios SOLID para máxima flexibilidad
- **Sensores Especializados:** Monitoreo completo de condiciones agrícolas
- **Energía Inteligente:** Sistema de gestión energética avanzado
- **Escalabilidad:** Soporte para hasta 100 nodos por red

Componentes del Sistema
----------------------
- **Unidades de Campo (Nodos):** ESP32 + SX1278 + GPS + Sensores Agrícolas
- **Gateway Central:** Procesamiento y almacenamiento de datos
- **Sistema de Comunicación:** Protocolo LoRa mesh personalizado

Arquitectura del Sistema
------------------------
.. include:: ../diagramas_codigo/architecture_diagram.mmd
   :parser: mermaid
   :align: center

Flujo de Datos
--------------
.. include:: ../diagramas_codigo/data_flow_diagram.mmd
   :parser: mermaid
   :align: center

Errores Comunes y Decisiones de Diseño
--------------------------------------
.. include:: errores_comunes.rst

Arquitectura de Capas (Visual)
------------------------------
.. mermaid::
   :align: center

   flowchart TD
     A["main.cpp / main_nodo.ino\n(Capa de Entrada Principal)"] --> B["AppLogic\n(Capa de Lógica de Aplicación)"]
     B --> C["SensorManager / NodeIdentity\n(Adquisición de Sensores / Identidad)"]
     B --> D["RadioManager\n(Capa de Radio)"]
     D --> E["LoRa Mesh Network"]
     B --> F["Protocol\n(Capa de Protocolo)"]
     C --> G["Sensores Físicos\n(DHT, GPS, NPK, etc.)"]
     style A fill:#e0f7fa
     style B fill:#ffe0b2
     style C fill:#c8e6c9
     style D fill:#d1c4e9
     style F fill:#fff9c4
     style G fill:#f8bbd0
     style E fill:#b3e5fc

Para más detalles técnicos, consulta las secciones de API y Especificaciones. 

Diagrama de Bloques de Hardware
------------------------------
.. mermaid::
   :align: center

   flowchart TD
     ESP32["ESP32 (MCU)"]
     LoRa["Módulo LoRa (SX1278)\nRFM95_CS: 5\nRFM95_INT: 26\nRFM95_RST: 27\nVSPI_SCK: 18\nVSPI_MISO: 19\nVSPI_MOSI: 23\nVSPI_SS: 5"]
     GPS["Módulo GPS (UART2)\nGPS_RX_UART2: 16\nGPS_TX_UART2: 17"]
     RS485["Módulo RS485 (SoftwareSerial)\nRS485_RX_SOFTWARE: 14\nRS485_TX_SOFTWARE: 15\nRS485_RE_DE: 2"]
     DHT["Sensor DHT (Temp/Hum)\nPIN_SENS_DHTT: 22"]
     BAT["Medición Batería\nBATTERY_VOLTAGE_ADC_PIN: 34"]

     ESP32 --> LoRa
     ESP32 --> GPS
     ESP32 --> RS485
     ESP32 --> DHT
     ESP32 --> BAT

     style ESP32 fill:#e3f2fd,stroke:#1976d2,stroke-width:2px
     style LoRa fill:#fffde7,stroke:#fbc02d
     style GPS fill:#e8f5e9,stroke:#388e3c
     style RS485 fill:#f3e5f5,stroke:#7b1fa2
     style DHT fill:#ffe0b2,stroke:#f57c00
     style BAT fill:#f8bbd0,stroke:#c2185b

   Diagrama de bloques de conexiones físicas entre el ESP32 y los periféricos principales según los pines definidos en config.h. 