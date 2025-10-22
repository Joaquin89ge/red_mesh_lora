#!/bin/bash

# Script para construir toda la documentación del proyecto
set -e  # Salir si cualquier comando falla

echo "=== Construyendo Sistema de Documentación Red Mesh ==="
echo "Fecha: $(date)"
echo "Directorio de trabajo: $(pwd)"
echo ""

# Función para construir documentación con mejor manejo de errores
build_docs() {
    local source_dir=$1
    local build_dir=$2
    local name=$3
    local conf_file="$source_dir/conf.py"
    
    echo "📚 Construyendo documentación: $name"
    echo "   📁 Directorio fuente: $source_dir"
    echo "   📁 Directorio destino: $build_dir"
    echo "   ⚙️  Archivo de configuración: $conf_file"
    
    if [ ! -d "$source_dir" ]; then
        echo "   ⚠️  ADVERTENCIA: Directorio $source_dir no encontrado, saltando..."
        return 1
    fi
    
    if [ ! -f "$conf_file" ]; then
        echo "   ❌ ERROR: Archivo conf.py no encontrado en $source_dir"
        return 1
    fi
    
    # Limpiar directorio de build anterior
    if [ -d "$build_dir" ]; then
        echo "   🧹 Limpiando build anterior..."
        # Cambiar permisos antes de eliminar
        find "$build_dir" -type f -exec chmod 644 {} \; 2>/dev/null || true
        find "$build_dir" -type d -exec chmod 755 {} \; 2>/dev/null || true
        rm -rf "$build_dir"
    fi
    
    # Crear directorio de destino si no existe
    mkdir -p "$build_dir"
    
    # Construir documentación con verbose output
    echo "   🔨 Ejecutando sphinx-build..."
    if sphinx-build -v -b html "$source_dir" "$build_dir"; then
        echo "   ✅ Documentación $name generada exitosamente"
        echo "   🌐 Acceso: $build_dir/index.html"
        return 0
    else
        echo "   ❌ ERROR: Falló la construcción de $name"
        return 1
    fi
}

# Contador de éxitos y fallos
success_count=0
fail_count=0

echo "🚀 Iniciando construcción de documentación..."
echo ""

# Construir documentación principal (red_mesh)
if build_docs "docs" "docs/_build/html" "Sistema Red Mesh Principal"; then
    ((success_count++))
else
    ((fail_count++))
fi

echo ""

# Construir documentación del gateway
if build_docs "main_gateway/docs" "main_gateway/docs/_build/html" "Gateway Agrícola"; then
    ((success_count++))
else
    ((fail_count++))
fi

echo ""

# Construir documentación de nodos
if build_docs "main_nodo/docs" "main_nodo/docs/_build/html" "Sistema de Nodos"; then
    ((success_count++))
else
    ((fail_count++))
fi

echo ""
echo "=== 📊 RESUMEN DE CONSTRUCCIÓN ==="
echo "✅ Exitosos: $success_count"
echo "❌ Fallidos: $fail_count"
echo ""

if [ $fail_count -eq 0 ]; then
    echo "🎉 ¡Toda la documentación se construyó exitosamente!"
    echo ""
    echo "📖 Documentación disponible en:"
    echo "   🔗 Principal: docs/_build/html/index.html"
    echo "   🔗 Gateway: main_gateway/docs/_build/html/index.html"
    echo "   🔗 Nodos: main_nodo/docs/_build/html/index.html"
    echo ""
    echo "🌐 Para servir la documentación:"
    echo "   docker compose --profile serve up doc_server"
    echo "   Luego visita: http://localhost:8000"
else
    echo "⚠️  Algunas documentaciones fallaron. Revisa los errores arriba."
    exit 1
fi
