API del Sistema de Monitoreo Agrícola
=====================================

Esta sección contiene la documentación completa de la API del sistema, generada automáticamente 
usando Sphinx con autodoc para garantizar precisión y actualización automática.

.. toctree::
   :maxdepth: 2
   :caption: Clases Principales:

   sensor_manager
   voltage_reader
   radio_manager
   app_logic
   node_identity
   protocol

Clases del Sistema
==================

SensorManager
-------------

.. automodule:: src.sensor_manager
   :members:
   :undoc-members:
   :show-inheritance:

VoltageReader
-------------

.. automodule:: src.voltage_reader
   :members:
   :undoc-members:
   :show-inheritance:

RadioManager
------------

.. automodule:: src.radio_manager
   :members:
   :undoc-members:
   :show-inheritance:

AppLogic
---------

.. automodule:: src.app_logic
   :members:
   :undoc-members:
   :show-inheritance:

NodeIdentity
------------

.. automodule:: src.node_identity
   :members:
   :undoc-members:
   :show-inheritance:

Protocol
---------

.. automodule:: src.protocol
   :members:
   :undoc-members:
   :show-inheritance:

Estructuras de Datos
====================

Protocol::AtmosphericSample
---------------------------

.. doxygenstruct:: Protocol::AtmosphericSample
   :members:
   :undoc-members:

Protocol::GroundSensor
---------------------

.. doxygenstruct:: Protocol::GroundSensor
   :members:
   :undoc-members:

Protocol::GpsSensor
------------------

.. doxygenstruct:: Protocol::GpsSensor
   :members:
   :undoc-members:

Protocol::EnergyData
-------------------

.. doxygenstruct:: Protocol::EnergyData
   :members:
   :undoc-members:

Configuración
============

Config
-------

.. automodule:: src.config
   :members:
   :undoc-members:
   :show-inheritance:

Ejemplos de Uso
===============

SensorManager Example
--------------------

.. literalinclude:: ../../src/sensor_manager_example.ino
   :language: cpp
   :caption: Ejemplo completo de uso del SensorManager

VoltageReader Example
--------------------

.. literalinclude:: ../../src/voltage_reader_example.ino
   :language: cpp
   :caption: Ejemplo de uso del VoltageReader

Notas de la API
===============

* Todas las clases siguen principios SOLID para máxima modularidad
* La documentación se genera automáticamente desde el código fuente
* Los ejemplos están probados y funcionan con la versión actual
* Para contribuir, ver :doc:`../developer_guide/contributing` 