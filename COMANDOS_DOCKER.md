# Guía de Comandos Docker para el Sistema de Documentación

## Opción 1: Docker Compose (Recomendado)

### Construir la documentación
```bash
# Construir toda la documentación del proyecto
docker compose --profile build up doc_builder

# O de forma más simple:
docker compose run --rm doc_builder
```

### Servir la documentación
```bash
# Después de construir, servir en http://localhost:8000
docker compose --profile serve up doc_server

# O usar el script interactivo que abre automáticamente el navegador
./update_all_docs.sh
# Selecciona opción 2: "Construir y servir documentación"
```

### Proceso completo en un comando
```bash
# Construir y servir automáticamente

docker compose --profile build up doc_builder && \
docker compose --profile serve up doc_server
```

## Opción 2: Script Interactivo (Recomendado para desarrollo)

### Usar el script interactivo
```bash
# Ejecutar el script interactivo con menú
./update_all_docs.sh
```

**Opciones disponibles:**
1. 🔨 Construir toda la documentación
2. 🌐 Construir y servir documentación (abre navegador automáticamente)
3. 📚 Construir solo documentación principal
4. 🏠 Construir solo documentación del gateway
5. 🔌 Construir solo documentación de nodos
6. 🧹 Limpiar todos los builds
7. 📖 Abrir navegador con documentación
8. ❌ Salir

**Características especiales:**
- ✅ Abre automáticamente el navegador en `http://localhost:8000/navegacion_docs.html`
- ✅ Compatible con Linux (xdg-open), macOS (open) y Windows (start)
- ✅ Muestra enlaces directos a toda la documentación
- ✅ Manejo automático del servidor HTTP

## Opción 3: Docker directo

### Construir la imagen
```bash
docker build -t mi-sphinx-doc .
```

### Ejecutar construcción de documentación
```bash
# Montar el directorio actual para que los archivos HTML se generen en tu máquina
docker run --rm -v $(pwd):/app mi-sphinx-doc
```

### Servir la documentación (alternativa)
```bash
# Usar el servidor Python integrado
docker run --rm -p 8000:8000 -v $(pwd):/app -w /app python:3.11-slim python -m http.server 8000
```

## Verificación de resultados

Después de ejecutar cualquiera de los comandos anteriores, deberías tener:

```
docs/_build/html/index.html              # Documentación principal (Red Mesh)
main_gateway/docs/_build/html/index.html  # Documentación del gateway agrícola
main_nodo/docs/_build/html/index.html     # Documentación del sistema de nodos
```

### Estructura de documentación generada:

1. **Sistema Red Mesh Principal** (`docs/`)
   - Arquitectura general del sistema
   - Especificaciones técnicas
   - Componentes principales

2. **Gateway Agrícola** (`main_gateway/docs/`)
   - Documentación del módulo gateway
   - Configuración MQTT
   - Gestión de radio y RTC

3. **Sistema de Nodos** (`main_nodo/docs/`)
   - Documentación de sensores
   - Protocolo de comunicación
   - APIs y ejemplos de uso

## Acceso a la documentación

1. **Navegación local**: Abre `navegacion_docs.html` en tu navegador
2. **Servidor HTTP**: Visita `http://localhost:8000` cuando uses el comando de servir
3. **Archivos directos**: Navega directamente a las carpetas `_build/html`

## Troubleshooting

### Si hay errores de permisos:
```bash
# Dar permisos de ejecución al script
chmod +x build_docs.sh
```

### Si necesitas limpiar builds anteriores:
```bash
# Limpiar builds previos
rm -rf docs/_build/html main_gateway/docs/_build/html main_nodo/docs/_build/html
```

### Para ver logs detallados:
```bash
docker compose run --rm doc_builder ./build_docs.sh
```
