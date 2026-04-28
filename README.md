# Sistema Red Mesh Agrícola

[![Docs Online](https://img.shields.io/badge/docs-online-blue.svg)](https://69f12b8b7fa0109a0fb4dc12--glowing-bienenstitch-a11f31.netlify.app/)
[![Documentación Local](https://img.shields.io/badge/docs-local-green.svg)](docs/_build/html/index.html)

Sistema avanzado para monitoreo y control agrícola basado en red mesh LoRa, integrando múltiples nodos sensores y gateways para recolección y análisis de datos en tiempo real.

## 🚀 Documentación

### 📖 Cómo Abrir la Documentación

**Opción 1: Script Automático (Recomendado)**
```bash
./update_all_docs.sh
# Selecciona opción 2: "🌐 Construir y servir documentación"
# Se abrirá automáticamente en http://localhost:8000/navegacion_docs.html
```

**Opción 2: Construcción Manual**
```bash
# Construir documentación
./build_docs.sh

# Servir en navegador
python3 -m http.server 8000
# Luego abre: http://localhost:8000/navegacion_docs.html
```

**Opción 3: Archivos Directos**
- **Navegación principal:** [navegacion_docs.html](navegacion_docs.html)
- **Documentación principal:** [docs/_build/html/index.html](docs/_build/html/index.html)
- **Gateway:** [main_gateway/docs/_build/html/index.html](main_gateway/docs/_build/html/index.html)
- **Nodos:** [main_nodo/docs/_build/html/index.html](main_nodo/docs/_build/html/index.html)

### 📚 Estructura de la Documentación

- **🏠 Página Principal:** Visión general del sistema
- **🏗️ Arquitectura:** Diseño y componentes del sistema
- **🔧 Stack IoT:** InfluxDB, Grafana, Node-RED, Mosquitto
- **📋 Especificaciones:** Detalles técnicos y hardware
- **⚙️ API:** Documentación de protocolos y funciones
- **📊 Métricas:** Rendimiento y KPIs del sistema

## 📚 Otras Documentaciones

- [Documentación main_gateway](main_gateway/docs/_build/html/index.html) (requiere servidor local)
- [Documentación main_nodo](main_nodo/docs/_build/html/index.html) (requiere servidor local)

## 🛠️ Generación de Documentación

### Scripts Disponibles

- **`./update_all_docs.sh`** - Script interactivo con menú completo
- **`./build_docs.sh`** - Construcción directa de toda la documentación
- **`./servir_docs.py`** - Servidor de documentación con interfaz web

### Dependencias

```bash
# Instalar dependencias de Sphinx
pip install sphinx sphinx_rtd_theme sphinxcontrib-mermaid myst-parser

# O usar el archivo requirements.txt
pip install -r requirements.txt
```

## 🤖 Actualización automática

La documentación se genera localmente y se actualiza con cada build.

## 📦 Estructura del proyecto

- `docs/` — Documentación general y técnica
- `main_gateway/` — Submódulo gateway
- `main_nodo/` — Submódulo nodo

## 🤝 Contribuir

Lee la [guía de colaboración](docs/colaboracion.rst) antes de enviar cambios o sugerencias.

---

**¡Explora la documentación y haz crecer el sistema Red Mesh Agrícola!**
