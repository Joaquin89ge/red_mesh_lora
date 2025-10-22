Documentación de la API
========================

Esta sección contiene la documentación automática de las clases, funciones y módulos principales del sistema Red Mesh Agrícola.

.. note::
   La documentación de la API se genera automáticamente a partir de los docstrings del código fuente usando la extensión autodoc de Sphinx.

.. toctree::
   :maxdepth: 2
   :caption: Módulos API

   protocol


Ejemplo de uso de autodoc
-------------------------

Para documentar un módulo o clase, añade en el archivo correspondiente:

.. automodule:: src.gateway
   :members:
   :undoc-members:
   :show-inheritance:

.. autoclass:: src.gateway.GatewayCentral
   :members:
   :undoc-members:
   :show-inheritance:

Reemplaza `src.gateway` y `GatewayCentral` por el nombre real de tu módulo/clase. 