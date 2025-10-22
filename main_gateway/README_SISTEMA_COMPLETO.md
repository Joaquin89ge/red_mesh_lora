# 🌱 Sistema Completo de Sensores Agrícolas - TD4

Sistema integrado completo para recolección, procesamiento y visualización de datos de sensores agrícolas ESP8266.

## 🎯 Descripción General

Este sistema incluye:

- **Sistema MQTT Python**: Recolecta datos de sensores ESP8266
- **Mosquitto Broker**: Distribuye mensajes MQTT
- **Dashboard Node-RED**: Visualización en tiempo real
- **Scripts de instalación**: Automatización completa

## 🚀 Inicio Rápido

### Windows

```bash
# Ejecutar todo el sistema con un solo comando
start_system.bat
```

### Linux/macOS

```bash
# Dar permisos de ejecución
chmod +x start_system.sh

# Ejecutar todo el sistema
./start_system.sh
```

## 📁 Estructura del Proyecto

```
main_gateway/
├── start_system.bat          # Script principal Windows
├── start_system.sh           # Script principal Linux/macOS
├── README_SISTEMA_COMPLETO.md # Esta documentación
├── mqtt_propio/             # Sistema MQTT Python
│   ├── main.py              # Aplicación principal
│   ├── config.py            # Configuración
│   ├── mqtt_client.py       # Cliente MQTT
│   ├── data_manager.py      # Gestor de datos
│   ├── mosquitto_manager.py # Gestor de Mosquitto
│   ├── requirements.txt     # Dependencias Python
│   ├── install.bat          # Instalador Windows
│   ├── install.sh           # Instalador Linux/macOS
│   └── README.md            # Documentación MQTT
├── nodered_dashboard/       # Dashboard Node-RED
│   ├── package.json         # Dependencias Node.js
│   ├── settings.js          # Configuración Node-RED
│   ├── flows.json           # Flujo principal
│   ├── dashboard.json       # Configuración dashboard
│   ├── install.bat          # Instalador Windows
│   ├── install.sh           # Instalador Linux/macOS
│   └── README.md            # Documentación Dashboard
└── src/                     # Código ESP8266 (Gateway)
    ├── main_gateway.ino     # Código principal
    ├── app_logic.cpp        # Lógica de aplicación
    ├── config.h             # Configuración
    └── ...                  # Otros archivos
```

## 🔧 Requisitos Previos

### Software Necesario

1. **Python 3.7+**: https://python.org
2. **Node.js 14+**: https://nodejs.org
3. **Arduino IDE**: Para programar ESP8266

### Hardware

- ESP8266 (NodeMCU o similar)
- Sensores agrícolas (DHT22, pH, CE, GPS, etc.)

## 🛠️ Instalación Manual (Si los scripts fallan)

### 1. Instalar Python y Node.js

```bash
# Verificar instalaciones
python --version
node --version
```

### 2. Instalar Sistema MQTT

```bash
cd mqtt_propio
pip install -r requirements.txt
python main.py --install-mosquitto
```

### 3. Instalar Node-RED Dashboard

```bash
cd nodered_dashboard
npm install
npm install -g node-red-dashboard
npm install -g node-red-contrib-mqtt-broker
npm install -g node-red-contrib-chartjs
npm install -g node-red-contrib-gauge
```

### 4. Ejecutar Servicios

```bash
# Terminal 1: Sistema MQTT
cd mqtt_propio
python main.py

# Terminal 2: Node-RED Dashboard
cd nodered_dashboard
node-red --settings settings.js
```

## 🌐 Acceso al Sistema

### Dashboard Principal

- **URL**: http://localhost:1880/ui
- **Usuario**: admin
- **Contraseña**: admin

### Editor Node-RED

- **URL**: http://localhost:1880
- **Usuario**: admin
- **Contraseña**: admin

### Sistema MQTT

- **Broker**: localhost:1883
- **Logs**: mqtt_propio/logs/
- **Datos**: mqtt_propio/data/

## 📊 Funcionalidades del Dashboard

### Pestañas Disponibles

#### 1. Dashboard Principal

- ✅ Resumen general del sistema
- ✅ Estado de conexión MQTT
- ✅ Contador de nodos activos
- ✅ Alertas y notificaciones

#### 2. Datos Atmosféricos

- ✅ Gráfico de temperatura y humedad en tiempo real
- ✅ Medidores para valores actuales
- ✅ Tabla con historial completo
- ✅ Estadísticas por nodo

#### 3. Datos de Suelo/GPS

- ✅ Gráfico de parámetros del suelo
- ✅ Medidores para pH, CE, temperatura, humedad
- ✅ Tabla con datos completos
- ✅ Información de ubicación GPS

#### 4. Configuración

- ✅ Configuración de MQTT
- ✅ Ajustes del dashboard
- ✅ Exportación de datos
- ✅ Logs del sistema

## 🔄 Flujo de Datos

```
ESP8266 (Sensores)
    ↓ (MQTT)
Mosquitto Broker (localhost:1883)
    ↓ (Topics)
Sistema MQTT Python (Recolección)
    ↓ (Procesamiento)
Node-RED Dashboard (Visualización)
    ↓ (Tiempo real)
Dashboard Web (http://localhost:1880/ui)
```

## 📱 Características del Dashboard

### Visualización

- **Gráficos interactivos** con Chart.js
- **Medidores coloridos** según rangos
- **Tablas responsivas** con filtros
- **Tema agrícola** personalizado

### Funcionalidades

- **Tiempo real**: Actualización automática
- **Responsive**: Funciona en móviles
- **Exportación**: CSV, JSON
- **Backup automático**: Cada 5 minutos

### Alertas

- **Temperatura alta**: >35°C
- **Humedad baja**: <30%
- **pH crítico**: <5.5 o >8.5
- **Conectividad**: Nodos desconectados

## 🔧 Configuración

### Configuración WiFi (ESP8266)

```cpp
// En config.h - NO CAMBIAR
#define WIFI_SSID "TuRedWiFi"
#define WIFI_PASSWORD "TuPassword"
```

### Configuración MQTT

```python
# En mqtt_propio/config.py
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
```

### Topics MQTT

- `sensor/+/atmospheric` - Datos atmosféricos
- `sensor/+/ground_gps` - Datos de suelo/GPS
- `sensor/+/announce` - Anuncios de nodos

## 🛠️ Solución de Problemas

### Sistema no inicia

1. Verificar Python y Node.js instalados
2. Ejecutar scripts de instalación individuales
3. Revisar logs en las carpetas correspondientes

### No se conecta a MQTT

1. Verificar Mosquitto: `python main.py --status`
2. Probar conexión: `python main.py --test`
3. Verificar configuración de red WiFi

### Dashboard no muestra datos

1. Verificar que ESP8266 envíe datos
2. Confirmar topics MQTT correctos
3. Revisar logs de Node-RED

### Error de puertos

1. Verificar puerto 1880 libre (Node-RED)
2. Verificar puerto 1883 libre (Mosquitto)
3. Detener servicios conflictivos

## 📈 Monitoreo y Mantenimiento

### Logs del Sistema

- **MQTT**: `mqtt_propio/logs/`
- **Node-RED**: Consola del editor
- **Mosquitto**: Logs del sistema

### Backups Automáticos

- **Datos**: Cada 5 minutos
- **Ubicación**: `mqtt_propio/data/`
- **Formato**: JSON y CSV

### Limpieza de Datos

- **Automática**: 7 días por defecto
- **Manual**: Desde configuración
- **Exportación**: Antes de limpiar

## 🔒 Seguridad

### Configuración Actual (Desarrollo)

- **MQTT**: Sin autenticación
- **Node-RED**: admin/admin
- **Acceso**: Solo localhost

### Para Producción

1. Configurar autenticación MQTT
2. Cambiar credenciales Node-RED
3. Implementar HTTPS
4. Restringir acceso por IP

## 📞 Soporte

### Comandos Útiles

```bash
# Verificar estado de Mosquitto
python mqtt_propio/main.py --status

# Probar conexión MQTT
python mqtt_propio/main.py --test

# Verificar Node-RED
curl http://localhost:1880

# Ver logs del sistema
tail -f mqtt_propio/logs/*.log
```

### Recursos Adicionales

- **Documentación MQTT**: `mqtt_propio/README.md`
- **Documentación Dashboard**: `nodered_dashboard/README.md`
- **Código ESP8266**: `src/`

## 🎯 Próximos Pasos

1. **Ejecutar el sistema**: `start_system.bat` o `./start_system.sh`
2. **Acceder al dashboard**: http://localhost:1880/ui
3. **Programar ESP8266**: Cargar código desde `src/`
4. **Conectar sensores**: Seguir diagramas de conexión
5. **Verificar datos**: Los datos aparecerán automáticamente

## 🤝 Contribución

Para contribuir al proyecto:

1. Fork el repositorio
2. Crear rama para feature
3. Commit cambios
4. Push a la rama
5. Crear Pull Request

---

**🌱 Sistema de Sensores Agrícolas - TD4**  
_Desarrollado para Técnicas Digitales 4_
