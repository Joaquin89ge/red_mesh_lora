Sistema Red Mesh Agrícola
=========================

Documentación técnica unificada del sistema de red mesh para monitoreo y control agrícola.

.. mermaid::

   graph TB
       subgraph "Sistema Red Mesh Agrícola"
           A[Nodos Sensores] --> B[Gateway Central]
           B --> C[Procesamiento Local]
           C --> D[Almacenamiento]
           C --> E[Visualización]
           B --> F[Conexión a la Nube]
           
           subgraph "Sensores"
               G[Temperatura]
               H[Humedad]
               I[Presión]
               J[Luz]
           end
           
           A --> G
           A --> H
           A --> I
           A --> J
       end

.. toctree::
   :maxdepth: 2
   :caption: Índice de Documentación

   descripcion_general
   componentes
   arquitectura
   stack_iot
   especificaciones
   funcionalidades
   ejemplos_uso
   metricas
   recursos
   colaboracion
   api/index
   overview/index

Otras Documentaciones
----------------------

* `Documentación Gateway <http://localhost:8000/main_gateway/docs/_build/html/index.html>`_
* `Documentación Nodos <http://localhost:8000/main_nodo/docs/_build/html/index.html>`_

.. note::
   Esta documentación sigue el mismo estilo visual y estructura que la del “Sistema Gateway Agrícola”.

Indices y tablas
================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search` 