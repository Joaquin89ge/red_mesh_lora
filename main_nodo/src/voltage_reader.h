#ifndef VOLTAGE_READER_H
#define VOLTAGE_READER_H

#include <Arduino.h>
#include "config.h"

/**
 * @class VoltageReader
 * @brief Clase para lectura de voltaje del divisor resistivo en pin 34 del ESP32
 * 
 * Esta clase está optimizada para lecturas precisas de un divisor resistivo
 * que proporciona una señal de 2.5V a 0V, con configuración ADC_ATTEN_11db
 * para el rango 0-2.6V.
 * 
 * Características:
 * - Configuración desde config.h
 * - ADC_ATTEN_11db para rango 0-2.6V
 * - Filtrado de media móvil para estabilidad
 * - Uso eficiente de funciones map() de Arduino
 * - Calibración automática y manual
 * - Validación de lecturas
 */
class VoltageReader {
private:
    // Configuración desde config.h
    static const uint8_t VOLTAGE_PIN = 34;
    static const uint8_t ADC_RESOLUTION = 12;
    static constexpr float VREF = 3.3f;
    #define MAX_INPUT_VOLTAGE VOLTAGE_READER_MAX_INPUT_VOLTAGE
    #define MIN_INPUT_VOLTAGE VOLTAGE_READER_MIN_INPUT_VOLTAGE
    static const uint8_t FILTER_SAMPLES = 8;
    
    // Parámetros de calibración
    float calibrationOffset;    ///< Offset de calibración
    float calibrationSlope;     ///< Pendiente de calibración
    
    // Filtrado de media móvil
    float lastReadings[FILTER_SAMPLES]; ///< Buffer de lecturas anteriores
    uint8_t readingIndex;      ///< Índice actual en el buffer
    bool filterInitialized;     ///< Indica si el filtro está inicializado
    
    /**
     * @brief Lee el valor raw del ADC
     * @return Valor raw del ADC (0-4095)
     */
    uint16_t readRawADC();
    
    /**
     * @brief Convierte el valor raw del ADC a voltaje
     * @param rawValue Valor raw del ADC
     * @return Voltaje en voltios
     */
    float rawToVoltage(uint16_t rawValue);
    
    /**
     * @brief Aplica filtrado de media móvil
     * @param newValue Nueva lectura
     * @return Valor filtrado
     */
    float applyFilter(float newValue);

public:
    /**
     * @brief Constructor de la clase VoltageReader
     * @param offset Offset de calibración (por defecto 0.0)
     * @param slope Pendiente de calibración (por defecto 1.0)
     */
    VoltageReader(float offset = 0.0f, float slope = 1.0f);
    
    /**
     * @brief Inicializa el pin y configura el ADC
     */
    void begin();
    
    /**
     * @brief Lee el voltaje actual del divisor resistivo
     * @return Voltaje en voltios (0.0 a 2.5V)
     */
    float readVoltage();
    
    /**
     * @brief Lee el voltaje y lo mapea a un rango personalizado usando map()
     * @param minOutput Valor mínimo de salida
     * @param maxOutput Valor máximo de salida
     * @return Valor mapeado
     */
    float readVoltageMapped(float minOutput, float maxOutput);
    
    /**
     * @brief Lee el voltaje y lo mapea a porcentaje (0-100%) usando map()
     * @return Porcentaje (0.0 a 100.0)
     */
    float readVoltagePercentage();
    
    /**
     * @brief Lee el voltaje y lo mapea a un rango entero usando map()
     * @param minOutput Valor mínimo entero
     * @param maxOutput Valor máximo entero
     * @return Valor entero mapeado
     */
    int readVoltageMappedInt(int minOutput, int maxOutput);
    
    /**
     * @brief Obtiene el valor raw del ADC sin procesar
     * @return Valor raw del ADC (0-4095)
     */
    uint16_t readRawValue();
    
    /**
     * @brief Configura los parámetros de calibración
     * @param offset Offset de calibración
     * @param slope Pendiente de calibración
     */
    void setCalibration(float offset, float slope);
    
    /**
     * @brief Realiza una calibración automática
     * @param expectedMin Voltaje mínimo esperado
     * @param expectedMax Voltaje máximo esperado
     */
    void calibrate(float expectedMin, float expectedMax);
    
    /**
     * @brief Obtiene información de debug
     * @return String con información de debug
     */
    String getDebugInfo();
    
    /**
     * @brief Verifica si la lectura está en rango válido
     * @return true si la lectura es válida, false en caso contrario
     */
    bool isReadingValid();
    
    /**
     * @brief Obtiene el voltaje de referencia del ESP32
     * @return Voltaje de referencia en voltios
     */
    static float getVRef() { return VREF; }
    
    /**
     * @brief Obtiene la resolución del ADC
     * @return Resolución en bits
     */
    static uint8_t getADCResolution() { return ADC_RESOLUTION; }
    
    /**
     * @brief Obtiene el número de muestras del filtro
     * @return Número de muestras
     */
    static uint8_t getFilterSamples() { return FILTER_SAMPLES; }
};

#endif // VOLTAGE_READER_H 