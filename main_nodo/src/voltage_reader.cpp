#include "voltage_reader.h"

/**
 * @brief Constructor de la clase VoltageReader
 * @param offset Offset de calibración
 * @param slope Pendiente de calibración
 */
VoltageReader::VoltageReader(float offset, float slope)
    : calibrationOffset(offset), calibrationSlope(slope), readingIndex(0), filterInitialized(false)
{
    // Inicializar buffer de filtrado
    for (uint8_t i = 0; i < FILTER_SAMPLES; i++) {
        lastReadings[i] = 0.0f;
    }
}

/**
 * @brief Inicializa el pin y configura el ADC
 */
void VoltageReader::begin() {
    // Configurar resolución del ADC
    analogReadResolution(ADC_RESOLUTION);
    
    // Configurar atenuación del ADC para rango 0-2.6V
    // analogSetAttenuation(ADC_ATTENDB_MAX); // Comentado - usar atenuación por defecto
    
    // Configurar el pin como entrada analógica
    pinMode(VOLTAGE_PIN, INPUT);
    
    // Realizar algunas lecturas iniciales para estabilizar
    for (int i = 0; i < 5; i++) {
        readRawADC();
        delay(10);
    }
    
    Serial.println("[VoltageReader] Inicializado en pin " + String(VOLTAGE_PIN));
    Serial.println("[VoltageReader] Rango de entrada: " + String(MIN_INPUT_VOLTAGE) + "V a " + String(MAX_INPUT_VOLTAGE) + "V");
    Serial.println("[VoltageReader] Atenuación ADC: 11dB");
}

/**
 * @brief Lee el valor raw del ADC
 * @return Valor raw del ADC (0-4095)
 */
uint16_t VoltageReader::readRawADC() {
    return analogRead(VOLTAGE_PIN);
}

/**
 * @brief Convierte el valor raw del ADC a voltaje
 * @param rawValue Valor raw del ADC
 * @return Voltaje en voltios
 */
float VoltageReader::rawToVoltage(uint16_t rawValue) {
    // Convertir valor raw a voltaje (0-3.3V)
    float voltage = (rawValue * VREF) / ((1 << ADC_RESOLUTION) - 1);
    
    // Aplicar calibración
    voltage = (voltage * calibrationSlope) + calibrationOffset;
    
    return voltage;
}

/**
 * @brief Aplica filtrado de media móvil
 * @param newValue Nueva lectura
 * @return Valor filtrado
 */
float VoltageReader::applyFilter(float newValue) {
    // Agregar nueva lectura al buffer
    lastReadings[readingIndex] = newValue;
    readingIndex = (readingIndex + 1) % FILTER_SAMPLES;
    
    // Marcar filtro como inicializado después de la primera vuelta
    if (readingIndex == 0) {
        filterInitialized = true;
    }
    
    // Si el filtro no está inicializado, devolver el valor sin filtrar
    if (!filterInitialized) {
        return newValue;
    }
    
    // Calcular media móvil
    float sum = 0.0f;
    for (uint8_t i = 0; i < FILTER_SAMPLES; i++) {
        sum += lastReadings[i];
    }
    
    return sum / FILTER_SAMPLES;
}

/**
 * @brief Lee el voltaje actual del divisor resistivo
 * @return Voltaje en voltios (0.0 a 2.5V)
 */
float VoltageReader::readVoltage() {
    uint16_t rawValue = readRawADC();
    float voltage = rawToVoltage(rawValue);
    
    // Aplicar filtrado
    voltage = applyFilter(voltage);
    
    // Limitar al rango de entrada
    if (voltage > MAX_INPUT_VOLTAGE) voltage = MAX_INPUT_VOLTAGE;
    if (voltage < MIN_INPUT_VOLTAGE) voltage = MIN_INPUT_VOLTAGE;
    
    return voltage;
}

/**
 * @brief Lee el voltaje y lo mapea a un rango personalizado usando map()
 * @param minOutput Valor mínimo de salida
 * @param maxOutput Valor máximo de salida
 * @return Valor mapeado
 */
float VoltageReader::readVoltageMapped(float minOutput, float maxOutput) {
    float voltage = readVoltage();
    
    // Usar función map() de Arduino para mapeo eficiente
    // Mapear directamente de voltaje a rango de salida
    // 2.5V = minOutput, 0V = maxOutput
    return map(voltage, MAX_INPUT_VOLTAGE, MIN_INPUT_VOLTAGE, minOutput, maxOutput);
}

/**
 * @brief Lee el voltaje y lo mapea a porcentaje (0-100%) usando map()
 * @return Porcentaje (0.0 a 100.0)
 */
float VoltageReader::readVoltagePercentage() {
    float voltage = readVoltage();
    
    // Usar función map() de Arduino para mapeo eficiente
    // 2.5V = 0%, 0V = 100%
    return map(voltage, MAX_INPUT_VOLTAGE, MIN_INPUT_VOLTAGE, 0.0f, 100.0f);
}

/**
 * @brief Lee el voltaje y lo mapea a un rango entero usando map()
 * @param minOutput Valor mínimo entero
 * @param maxOutput Valor máximo entero
 * @return Valor entero mapeado
 */
int VoltageReader::readVoltageMappedInt(int minOutput, int maxOutput) {
    float voltage = readVoltage();
    
    // Usar función map() de Arduino para mapeo eficiente
    // 2.5V = minOutput, 0V = maxOutput
    return map(voltage, MAX_INPUT_VOLTAGE, MIN_INPUT_VOLTAGE, minOutput, maxOutput);
}

/**
 * @brief Obtiene el valor raw del ADC sin procesar
 * @return Valor raw del ADC (0-4095)
 */
uint16_t VoltageReader::readRawValue() {
    return readRawADC();
}

/**
 * @brief Configura los parámetros de calibración
 * @param offset Offset de calibración
 * @param slope Pendiente de calibración
 */
void VoltageReader::setCalibration(float offset, float slope) {
    calibrationOffset = offset;
    calibrationSlope = slope;
    
    Serial.println("[VoltageReader] Calibración actualizada:");
    Serial.println("  Offset: " + String(calibrationOffset));
    Serial.println("  Slope: " + String(calibrationSlope));
}

/**
 * @brief Realiza una calibración automática
 * @param expectedMin Voltaje mínimo esperado
 * @param expectedMax Voltaje máximo esperado
 */
void VoltageReader::calibrate(float expectedMin, float expectedMax) {
    Serial.println("[VoltageReader] Iniciando calibración automática...");
    Serial.println("  Voltaje mínimo esperado: " + String(expectedMin) + "V");
    Serial.println("  Voltaje máximo esperado: " + String(expectedMax) + "V");
    
    // Tomar múltiples lecturas para estabilizar
    float sumMin = 0.0f, sumMax = 0.0f;
    const int samples = 10;
    
    Serial.println("  Tomando " + String(samples) + " muestras para calibración...");
    
    for (int i = 0; i < samples; i++) {
        uint16_t rawValue = readRawADC();
        float voltage = (rawValue * VREF) / ((1 << ADC_RESOLUTION) - 1);
        
        sumMin += voltage;
        delay(100);
    }
    
    // Calcular parámetros de calibración
    float measuredMin = sumMin / samples;
    float measuredMax = MAX_INPUT_VOLTAGE; // Asumimos que el máximo es 2.5V
    
    // Calcular pendiente y offset
    calibrationSlope = (expectedMax - expectedMin) / (measuredMax - measuredMin);
    calibrationOffset = expectedMin - (measuredMin * calibrationSlope);
    
    Serial.println("[VoltageReader] Calibración completada:");
    Serial.println("  Voltaje medido mínimo: " + String(measuredMin) + "V");
    Serial.println("  Voltaje medido máximo: " + String(measuredMax) + "V");
    Serial.println("  Offset calculado: " + String(calibrationOffset));
    Serial.println("  Slope calculado: " + String(calibrationSlope));
}

/**
 * @brief Obtiene información de debug
 * @return String con información de debug
 */
String VoltageReader::getDebugInfo() {
    uint16_t rawValue = readRawADC();
    float voltage = rawToVoltage(rawValue);
    float percentage = readVoltagePercentage();
    
    String info = "=== VoltageReader Debug Info ===\n";
    info += "Pin: " + String(VOLTAGE_PIN) + "\n";
    info += "ADC Resolution: " + String(ADC_RESOLUTION) + " bits\n";
    info += "ADC Attenuation: 11dB\n";
    info += "VRef: " + String(VREF) + "V\n";
    info += "Raw ADC Value: " + String(rawValue) + "\n";
    info += "Voltage: " + String(voltage, 3) + "V\n";
    info += "Percentage: " + String(percentage, 1) + "%\n";
    info += "Calibration Offset: " + String(calibrationOffset) + "\n";
    info += "Calibration Slope: " + String(calibrationSlope) + "\n";
    info += "Input Range: " + String(MIN_INPUT_VOLTAGE) + "V to " + String(MAX_INPUT_VOLTAGE) + "V\n";
    info += "Filter Samples: " + String(FILTER_SAMPLES) + "\n";
    info += "Filter Initialized: " + String(filterInitialized ? "Yes" : "No") + "\n";
    info += "Valid Reading: " + String(isReadingValid() ? "Yes" : "No") + "\n";
    info += "================================";
    
    return info;
}

/**
 * @brief Verifica si la lectura está en rango válido
 * @return true si la lectura es válida, false en caso contrario
 */
bool VoltageReader::isReadingValid() {
    float voltage = readVoltage();
    return (voltage >= MIN_INPUT_VOLTAGE && voltage <= MAX_INPUT_VOLTAGE);
} 