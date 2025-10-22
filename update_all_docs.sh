#!/bin/bash

# Script para actualizar toda la documentación del proyecto
# Este script es más interactivo y útil para desarrollo

set -e

echo "🔄 Actualizando Documentación del Sistema Red Mesh"
echo "=================================================="
echo ""

# Función para mostrar el menú
show_menu() {
    echo "📋 Selecciona una opción:"
    echo ""
    echo "1. 🔨 Construir toda la documentación (recomendado)"
    echo "2. 🌐 Construir y servir documentación"
    echo "3. 📚 Construir solo documentación principal"
    echo "4. 🏠 Construir solo documentación del gateway"
    echo "5. 🔌 Construir solo documentación de nodos"
    echo "6. 🧹 Limpiar todos los builds"
    echo "7. 📖 Abrir navegador con documentación"
    echo "8. ❌ Salir"
    echo ""
}

# Función para construir documentación específica
build_specific() {
    local project=$1
    local name=$2
    
    echo "🔨 Construyendo $name..."
    
    case $project in
        "main")
            docker compose run --rm doc_builder bash -c "sphinx-build -v -b html docs docs/_build/html"
            ;;
        "gateway")
            docker compose run --rm doc_builder bash -c "sphinx-build -v -b html main_gateway/docs main_gateway/docs/_build/html"
            ;;
        "nodo")
            docker compose run --rm doc_builder bash -c "sphinx-build -v -b html main_nodo/docs main_nodo/docs/_build/html"
            ;;
        "all")
            docker compose run --rm doc_builder
            ;;
    esac
    
    echo "✅ $name construido exitosamente!"
}

# Función para limpiar builds
clean_builds() {
    echo "🧹 Limpiando builds anteriores..."
    rm -rf docs/_build/html main_gateway/docs/_build/html main_nodo/docs/_build/html
    echo "✅ Builds limpiados!"
}

# Función para abrir navegador
open_browser() {
    if command -v xdg-open > /dev/null; then
        xdg-open navegacion_docs.html
    elif command -v open > /dev/null; then
        open navegacion_docs.html
    else
        echo "⚠️  No se puede abrir el navegador automáticamente."
        echo "   Abre manualmente: navegacion_docs.html"
    fi
}

# Función para servir documentación
serve_docs() {
    echo "🌐 Iniciando servidor de documentación..."
    echo "   El servidor estará disponible en: http://localhost:8000"
    echo ""
    
    # Iniciar servidor en background
    python3 -m http.server 8000 &
    SERVER_PID=$!
    
    # Esperar un momento para que el servidor inicie
    sleep 3
    
    # Abrir navegador automáticamente
    echo "🚀 Abriendo navegador en http://localhost:8000/navegacion_docs.html..."
    if command -v xdg-open > /dev/null; then
        xdg-open http://localhost:8000/navegacion_docs.html
    elif command -v open > /dev/null; then
        open http://localhost:8000/navegacion_docs.html
    elif command -v start > /dev/null; then
        start http://localhost:8000/navegacion_docs.html
    else
        echo "⚠️  No se puede abrir el navegador automáticamente."
        echo "   Abre manualmente: http://localhost:8000/navegacion_docs.html"
    fi
    
    echo ""
    echo "📖 Documentación disponible en:"
    echo "   🔗 Navegación principal: http://localhost:8000/navegacion_docs.html"
    echo "   🔗 Principal: http://localhost:8000/docs/_build/html/index.html"
    echo "   🔗 Gateway: http://localhost:8000/main_gateway/docs/_build/html/index.html"
    echo "   🔗 Nodos: http://localhost:8000/main_nodo/docs/_build/html/index.html"
    echo ""
    echo "   Presiona Ctrl+C para detener el servidor"
    
    # Esperar a que el usuario detenga el servidor
    wait $SERVER_PID
}

# Bucle principal del menú
while true; do
    show_menu
    read -p "👉 Ingresa tu opción (1-8): " choice
    echo ""
    
    case $choice in
        1)
            echo "🚀 Construyendo toda la documentación..."
            build_specific "all" "Toda la documentación"
            ;;
        2)
            echo "🚀 Construyendo y sirviendo documentación..."
            build_specific "all" "Toda la documentación"
            echo ""
            serve_docs
            ;;
        3)
            build_specific "main" "Documentación principal"
            ;;
        4)
            build_specific "gateway" "Documentación del gateway"
            ;;
        5)
            build_specific "nodo" "Documentación de nodos"
            ;;
        6)
            clean_builds
            ;;
        7)
            open_browser
            ;;
        8)
            echo "👋 ¡Hasta luego!"
            exit 0
            ;;
        *)
            echo "❌ Opción inválida. Por favor, selecciona 1-8."
            ;;
    esac
    
    echo ""
    read -p "¿Deseas continuar? (y/n): " continue_choice
    if [[ ! "$continue_choice" =~ ^[Yy]$ ]]; then
        echo "👋 ¡Hasta luego!"
        exit 0
    fi
    echo ""
done
