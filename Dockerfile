# Usa una imagen oficial de Python, ligera y limpia.
FROM python:3.11-slim

# Establecer el directorio de trabajo dentro del contenedor
WORKDIR /app

# Instalar dependencias del sistema necesarias para compilar extensiones Python
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    && rm -rf /var/lib/apt/lists/*

# Copiar archivos de configuración
COPY requirements.txt .
COPY build_docs.sh .

# Instalar las dependencias de Sphinx según tu conf.py
# Incluye todas las extensiones que tienes configuradas
RUN pip install --no-cache-dir -r requirements.txt

# Hacer ejecutable el script de construcción
RUN chmod +x build_docs.sh

# Crear directorios necesarios para la documentación
RUN mkdir -p /app/docs/_build/html \
    /app/main_gateway/docs/_build/html \
    /app/main_nodo/docs/_build/html \
    /app/docs/_static \
    /app/docs/_templates

# Establecer variables de entorno para Sphinx
ENV SPHINXOPTS="-W"  # Tratar warnings como errores
ENV PYTHONPATH="/app"

# Comando por defecto para construir la documentación
CMD ["./build_docs.sh"]

