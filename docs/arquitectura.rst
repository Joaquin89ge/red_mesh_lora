Arquitectura del Sistema
========================

La arquitectura del sistema Red Mesh Agrícola está basada en una topología mesh con un gateway central y múltiples nodos sensores distribuidos.

.. mermaid:: diagramas_codigo/flujo_datos.mmd

**Descripción:**

- El gateway central coordina la red y comunica los datos a la nube.
- Los nodos sensores se comunican entre sí y con el gateway usando LoRa mesh.
- El sistema es tolerante a fallos y escalable, permitiendo la adición de nuevos nodos fácilmente.
- La arquitectura modular facilita el mantenimiento y la integración de nuevas funcionalidades. 

Topologia y gestion de red implementada:

Topología de Malla Parcial (Partial Mesh Network):
Algunos nodos están conectados a todos los demás, mientras que otros solo están conectados a los nodos vecinos con los que más interactúan.

        1. Auto-Organización (Self-Organizing ):
        Funcionamiento: Cuando se añade un nodo nuevo, este encuentra automáticamente al router principal o a un nodo ya existente, 
        se une a la red y comienza a retransmitir la señal, ajustando su configuración de forma autónoma.

        2. Auto-Reparación (Self-Healing):
        Funcionamiento: Si un nodo o una ruta de comunicación falla, la red es capaz de encontrar y establecer automáticamente una nueva ruta o conexión alternativa para los datos,
        manteniendo la conectividad sin interrupción (enrutamiento dinámico).
