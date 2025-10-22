/**
 * @file voltage_reader_example.ino
 * @brief Ejemplo completo de uso de la clase VoltageReader
 * 
 * Este ejemplo muestra cómo usar la clase VoltageReader para leer
 * un divisor resistivo conectado al pin 34 del ESP32 que proporciona
 * una señal de 2.5V a 0V.
 * 
 * Características demostradas:
 * - Configuración desde config.h
 * - ADC_ATTEN_DB_11 para rango 0-2.6V
 * - Uso eficiente de funciones map() de Arduino
 * - Filtrado de media móvil para estabilidad
 * - Calibración automática y manual
 * - Diferentes tipos de mapeo
 * - Validación de lecturas
 * - Información de debug detallada
 */

#include "voltage_reader.h"

// Crear instancia del lector de voltaje
VoltageReader voltageReader;

// Variables para el ejemplo
unsigned long lastPrintTime = 0;
const unsigned long PRINT_INTERVAL = 1000; // Imprimir cada segundo
unsigned long exampleCounter = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("=== VoltageReader Example ===");
    Serial.println("Leyendo divisor resistivo en pin 34");
    Serial.println("Rango de entrada: 2.5V a 0V");
    Serial.println("Configuración: ADC_ATTEN_DB_11 (0-2.6V)");
    Serial.println();
    
    // Inicializar el lector de voltaje
    voltageReader.begin();
    
    // Opcional: Realizar calibración automática
    // voltageReader.calibrate(0.0, 2.5);
    
    // Opcional: Configurar calibración manual
    // voltageReader.setCalibration(0.0, 1.0);
    
    Serial.println("VoltageReader inicializado correctamente");
    Serial.println();
    
    // Mostrar información de debug inicial
    Serial.println(voltageReader.getDebugInfo());
    Serial.println();
}

void loop() {
    // Leer voltaje cada segundo
    if (millis() - lastPrintTime >= PRINT_INTERVAL) {
        lastPrintTime = millis();
        exampleCounter++;
        
        // Ejemplo 1: Lectura básica
        ejemploLecturaBasica();
        
        // Ejemplo 2: Diferentes tipos de mapeo
        ejemploMapeo();
        
        // Ejemplo 3: Validación y debug
        ejemploValidacion();
        
        // Ejemplo 4: Aplicaciones específicas
        ejemploAplicaciones();
        
        // Mostrar información de debug cada 10 segundos
        if (exampleCounter % 10 == 0) {
            Serial.println(voltageReader.getDebugInfo());
            Serial.println();
        }
        
        Serial.println("----------------------------------------");
    }
}

/**
 * @brief Ejemplo 1: Lectura básica de voltaje
 */
void ejemploLecturaBasica() {
    Serial.println("=== Ejemplo 1: Lectura Básica ===");
    
    // Lectura directa de voltaje
    float voltage = voltageReader.readVoltage();
    Serial.println("Voltaje: " + String(voltage, 3) + "V");
    
    // Lectura de valor raw del ADC
    uint16_t rawValue = voltageReader.readRawValue();
    Serial.println("Valor Raw ADC: " + String(rawValue));
    
    // Lectura de porcentaje
    float percentage = voltageReader.readVoltagePercentage();
    Serial.println("Porcentaje: " + String(percentage, 1) + "%");
    
    Serial.println();
}

/**
 * @brief Ejemplo 2: Diferentes tipos de mapeo usando map()
 */
void ejemploMapeo() {
    Serial.println("=== Ejemplo 2: Mapeo con map() ===");
    
    // Mapeo a rango float personalizado
    float mappedFloat = voltageReader.readVoltageMapped(0.0, 100.0);
    Serial.println("Mapeado (0.0-100.0): " + String(mappedFloat, 1));
    
    // Mapeo a rango entero
    int mappedInt = voltageReader.readVoltageMappedInt(0, 255);
    Serial.println("Mapeado (0-255): " + String(mappedInt));
    
    // Mapeo a rango de temperatura (ejemplo)
    float temperature = voltageReader.readVoltageMapped(-10.0, 50.0);
    Serial.println("Temperatura simulada: " + String(temperature, 1) + "°C");
    
    // Mapeo a rango de humedad (ejemplo)
    int humidity = voltageReader.readVoltageMappedInt(0, 100);
    Serial.println("Humedad simulada: " + String(humidity) + "%");
    
    Serial.println();
}

/**
 * @brief Ejemplo 3: Validación y verificación de lecturas
 */
void ejemploValidacion() {
    Serial.println("=== Ejemplo 3: Validación ===");
    
    // Verificar si la lectura es válida
    bool isValid = voltageReader.isReadingValid();
    Serial.println("Lectura válida: " + String(isValid ? "Sí" : "No"));
    
    // Mostrar información de configuración
    Serial.println("Configuración:");
    Serial.println("  Pin: " + String(voltageReader.getVRef()) + "V");
    Serial.println("  Resolución ADC: " + String(voltageReader.getADCResolution()) + " bits");
    Serial.println("  Muestras filtro: " + String(voltageReader.getFilterSamples()));
    
    // Información de calibración
    float voltage = voltageReader.readVoltage();
    if (voltage < 0.1) {
        Serial.println("⚠️  Advertencia: Voltaje muy bajo");
    } else if (voltage > 2.4) {
        Serial.println("⚠️  Advertencia: Voltaje muy alto");
    } else {
        Serial.println("✅ Voltaje en rango normal");
    }
    
    Serial.println();
}

/**
 * @brief Ejemplo 4: Aplicaciones específicas
 */
void ejemploAplicaciones() {
    Serial.println("=== Ejemplo 4: Aplicaciones ===");
    
    // Aplicación 1: Medir nivel de batería
    float batteryLevel = voltageReader.readVoltagePercentage();
    Serial.println("Nivel de batería: " + String(batteryLevel, 1) + "%");
    
    // Aplicación 2: Sensor de humedad del suelo
    float soilMoisture = voltageReader.readVoltageMapped(0.0, 100.0);
    Serial.println("Humedad del suelo: " + String(soilMoisture, 1) + "%");
    
    // Aplicación 3: Sensor de luz
    int lightLevel = voltageReader.readVoltageMappedInt(0, 1023);
    Serial.println("Nivel de luz: " + String(lightLevel));
    
    // Aplicación 4: Detectar umbrales
    float voltage = voltageReader.readVoltage();
    if (voltage < 0.5) {
        Serial.println("🔴 Estado: CRÍTICO (voltaje muy bajo)");
    } else if (voltage < 1.0) {
        Serial.println("🟡 Estado: ADVERTENCIA (voltaje bajo)");
    } else if (voltage < 2.0) {
        Serial.println("🟢 Estado: NORMAL");
    } else {
        Serial.println("🔵 Estado: ALTO (voltaje elevado)");
    }
    
    Serial.println();
}

/**
 * @brief Ejemplo de calibración automática
 */
void ejemploCalibracion() {
    Serial.println("=== Ejemplo de Calibración ===");
    
    Serial.println("Realizando calibración automática...");
    voltageReader.calibrate(0.0, 2.5);
    
    Serial.println("Información después de calibración:");
    Serial.println(voltageReader.getDebugInfo());
    
    Serial.println("Calibración completada");
    Serial.println();
}

/**
 * @brief Ejemplo de calibración manual
 */
void ejemploCalibracionManual() {
    Serial.println("=== Ejemplo de Calibración Manual ===");
    
    // Configurar calibración personalizada
    voltageReader.setCalibration(0.1, 0.98);
    
    Serial.println("Calibración manual configurada:");
    Serial.println("  Offset: 0.1V");
    Serial.println("  Slope: 0.98");
    
    // Verificar efecto de la calibración
    float voltage = voltageReader.readVoltage();
    Serial.println("Voltaje calibrado: " + String(voltage, 3) + "V");
    
    Serial.println();
}

/**
 * @brief Ejemplo de monitoreo continuo
 */
void ejemploMonitoreoContinuo() {
    static unsigned long lastChangeTime = 0;
    static float lastVoltage = 0.0;
    
    float currentVoltage = voltageReader.readVoltage();
    float change = abs(currentVoltage - lastVoltage);
    
    // Detectar cambios significativos (>0.1V)
    if (change > 0.1) {
        Serial.println("🔄 Cambio detectado: " + String(lastVoltage, 3) + "V → " + String(currentVoltage, 3) + "V");
        lastChangeTime = millis();
        lastVoltage = currentVoltage;
    }
    
    // Mostrar tiempo desde último cambio
    unsigned long timeSinceChange = millis() - lastChangeTime;
    if (timeSinceChange > 5000) { // Cada 5 segundos
        Serial.println("⏱️  Tiempo desde último cambio: " + String(timeSinceChange / 1000) + "s");
    }
} 