Errores Comunes y Decisiones de Diseño
======================================

Durante el desarrollo del sistema de monitoreo agrícola, se presentaron diversos desafíos técnicos y de integración. Aquí se documentan los errores más frecuentes, los llamados "errores fantasma" y las decisiones clave tomadas para robustecer el sistema.

Errores Comunes
---------------
- **Problemas de comunicación LoRa:**
  - Causados por interferencias, mala configuración de pines o falta de ACK.
  - Solución: Verificar conexiones, reiniciar el módulo y ajustar parámetros en `config.h`.

- **Desbordamiento de buffers:**
  - Ocurre si no se controla el tamaño de los arrays de muestras.
  - Solución: Uso de defines y verificaciones en el código.

- **Lecturas inválidas de sensores:**
  - Por desconexión, mal cableado o sensores defectuosos.
  - Solución: Manejo de errores y valores por defecto en las estructuras.

- **Pérdida de datos GPS con SoftwareSerial:**
  - El módulo GPS envía datos continuamente sin solicitud previa.
  - Con SoftwareSerial se perdían datos críticos de ubicación.
  - **Solución:** Cambiar a HardwareSerial (UART2) para GPS.
  - **Configuración:** Pines 16 (RX), 17 (TX) para UART2.

- **Errores de comunicación RS485:**
  - Timeouts en comunicación con módulo sensor externo.
  - Datos corruptos o incompletos.
  - **Solución:** Verificar conexiones, ajustar timeouts y usar protocolo simple "ILF".
  - **Configuración:** Pines 14 (RX), 15 (TX), 2 (Control DE/RE).

Errores Fantasma
----------------
- **Reseteos inesperados del ESP32:**
  - A veces causados por watchdog, otras por picos de consumo o fallos de memoria.
  - Decisión: Implementar el Task Watchdog Timer (TWDT) y documentar los síntomas.

- **Datos GPS nulos tras reinicio:**
  - El GPS puede tardar 30-60 segundos en obtener señal tras un cold start.
  - Decisión: Alimentar el GPS de forma independiente para hot start.

- **Mensajes que no llegan al gateway:**
  - Puede ser por tablas de enrutamiento perdidas o gateway fuera de línea.
  - Decisión: Reintentos automáticos y logging detallado.

Decisiones de Diseño
--------------------
- **Arquitectura por capas:**
  - Separación clara entre lógica de aplicación, comunicación, sensores y protocolo.

- **Configuración centralizada:**
  - Uso de `config.h` para todos los parámetros críticos.

- **Buffers circulares y manejo de errores:**
  - Para evitar pérdida de datos y facilitar el debug.

- **Documentación y pruebas continuas:**
  - Uso de Sphinx y ejemplos prácticos para asegurar mantenibilidad.

- **Comunicación Serial Optimizada:**
  - **GPS (HardwareSerial UART2):** El módulo GPS envía datos continuamente sin solicitud previa. 
    Con SoftwareSerial se perdían datos críticos de ubicación. HardwareSerial garantiza captura completa.
  - **RS485 (SoftwareSerial):** Comunicación bajo demanda (solicitamos datos cuando los necesitamos). 
    SoftwareSerial es suficiente para comunicación controlada y permite flexibilidad en pines.
  - **Configuración:** GPS (pines 16,17) + RS485 (pines 14,15, control 2)

.. note::
   **Lección Aprendida:** Para dispositivos que envían datos continuamente (GPS), usar HardwareSerial. 
   Para comunicación controlada bajo demanda (RS485), SoftwareSerial es suficiente.

.. note::
   Si encuentras un error no documentado aquí, por favor repórtalo para mejorar el sistema. 