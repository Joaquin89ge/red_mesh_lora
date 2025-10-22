#!/bin/bash

echo "========================================"
echo "Sistema de Sensores Agrícolas - TD4"
echo "========================================"
echo
echo "Iniciando instalación y configuración completa..."
echo

# Verificar si Python está instalado
echo "[1/6] Verificando Python..."
if ! command -v python3 &> /dev/null; then
    echo "ERROR: Python3 no está instalado"
    echo "Por favor, instala Python desde https://python.org"
    exit 1
fi
echo "✓ Python encontrado: $(python3 --version)"
echo

# Verificar si Node.js está instalado
echo "[2/6] Verificando Node.js..."
if ! command -v node &> /dev/null; then
    echo "ERROR: Node.js no está instalado"
    echo "Por favor, instala Node.js desde https://nodejs.org"
    exit 1
fi
echo "✓ Node.js encontrado: $(node --version)"
echo

# Instalar dependencias del sistema MQTT
echo "[3/6] Instalando sistema MQTT..."
cd mqtt_propio
mkdir -p data logs
pip3 install -r requirements.txt
if [ $? -ne 0 ]; then
    echo "ERROR: No se pudieron instalar las dependencias Python"
    exit 1
fi
echo "✓ Sistema MQTT instalado"
cd ..
echo

# Instalar Node-RED Dashboard
echo "[4/6] Instalando Node-RED Dashboard..."
cd nodered_dashboard
mkdir -p data logs
npm install
if [ $? -ne 0 ]; then
    echo "ERROR: No se pudieron instalar las dependencias Node.js"
    exit 1
fi
echo "✓ Node-RED Dashboard instalado"
cd ..
echo

# Verificar e instalar Mosquitto
echo "[5/6] Verificando Mosquitto..."
cd mqtt_propio
python3 main.py --status
if [ $? -ne 0 ]; then
    echo "Mosquitto no está instalado. Intentando instalación automática..."
    python3 main.py --install-mosquitto
    if [ $? -ne 0 ]; then
        echo
        echo "Instalación automática de Mosquitto falló."
        echo "Por favor, instala Mosquitto manualmente:"
        echo "  Ubuntu/Debian: sudo apt-get install mosquitto"
        echo "  CentOS/RHEL: sudo yum install mosquitto"
        echo "  macOS: brew install mosquitto"
        echo
    fi
fi
cd ..
echo

# Iniciar servicios
echo "[6/6] Iniciando servicios..."
echo

# Función para limpiar procesos al salir
cleanup() {
    echo
    echo "Deteniendo servicios..."
    pkill -f "python.*main.py"
    pkill -f "node-red"
    pkill -f "mosquitto"
    echo "Servicios detenidos"
    exit 0
}

# Configurar trap para limpiar al salir
trap cleanup SIGINT SIGTERM

# Iniciar Mosquitto en segundo plano
echo "Iniciando Mosquitto..."
cd mqtt_propio
python3 main.py --start-mosquitto &
MOSQUITTO_PID=$!
sleep 3
cd ..

# Iniciar sistema MQTT en segundo plano
echo "Iniciando sistema MQTT..."
cd mqtt_propio
python3 main.py &
MQTT_PID=$!
sleep 5
cd ..

# Iniciar Node-RED en segundo plano
echo "Iniciando Node-RED Dashboard..."
cd nodered_dashboard
node-red --settings settings.js &
NODERED_PID=$!
sleep 10
cd ..

echo
echo "========================================"
echo "Sistema iniciado exitosamente!"
echo "========================================"
echo
echo "Servicios disponibles:"
echo
echo "🌐 Dashboard Node-RED:"
echo "   http://localhost:1880/ui"
echo "   Usuario: admin"
echo "   Contraseña: admin"
echo
echo "⚙️ Editor Node-RED:"
echo "   http://localhost:1880"
echo
echo "📊 Sistema MQTT:"
echo "   Puerto: 1883"
echo "   Logs: mqtt_propio/logs/"
echo "   Datos: mqtt_propio/data/"
echo
echo "📁 Archivos de configuración:"
echo "   - mqtt_propio/config.py"
echo "   - nodered_dashboard/settings.js"
echo
echo "🔧 Para detener el sistema:"
echo "   Presiona Ctrl+C en esta ventana"
echo "   O ejecuta: pkill -f 'python.*main.py' && pkill -f 'node-red'"
echo
echo "📱 El dashboard es responsivo y funciona en móviles"
echo
echo "========================================"
echo "Sistema listo para recibir datos de ESP8266"
echo "========================================"
echo
echo "Esperando datos de sensores..."
echo "(Los datos aparecerán automáticamente en el dashboard)"
echo

# Mantener el script ejecutándose
while true; do
    sleep 1
done 