# Prompt para Documentación de Código con Autodoc y Sphinx

## 🎯 Objetivo Principal
Crear documentación profesional y técnica para el proyecto de red mesh LoRa agrícola, específicamente para la **Unidad de Campo** (nodo sensor ESP32), utilizando Sphinx con autodoc para generar documentación automática de la API.

## 📋 Contexto del Proyecto

### Sistema General
- **Red mesh LoRa** para monitoreo agrícola
- **Nodos autónomos** con ESP32 + SX1278
- **Sensores:** GPS, DHT, NPK, pH, EC, temperatura, humedad, energía
- **Arquitectura modular** siguiendo principios SOLID
- **Comunicación LoRa** para transmisión de datos al gateway central

### Unidad de Campo (Nodo Sensor)
- **Hardware:** ESP32 + SX1278 + GPS + DHT + sensores agrícolas
- **Firmware:** Arquitectura modular con clases especializadas
- **Funcionalidad:** Adquisición, procesamiento y transmisión de datos sensoriales

## 📚 Estructura de Documentación Deseada

### 1. Documentación Técnica Profesional
- **Resumen ejecutivo** con características técnicas avanzadas
- **Arquitectura del sistema** con diagramas
- **API completa** generada automáticamente con autodoc
- **Guías de usuario** detalladas
- **Guías de desarrollador** con ejemplos de código
- **Solución de problemas** y troubleshooting

### 2. Elementos Visuales e Interactivos
- **Menús de navegación** jerárquicos
- **Listas organizadas** por categorías
- **Imágenes y diagramas** técnicos
- **Referencias cruzadas** entre secciones
- **Elementos interactivos** (ejemplos ejecutables, enlaces)

### 3. Estándares de Documentación
- **Formato Sphinx** con reStructuredText
- **Autodoc** para documentación automática de clases y métodos
- **Docstrings** en formato Google/NumPy
- **Comentarios Doxygen** en código C++
- **Ejemplos de código** ejecutables
- **Referencias técnicas** y bibliografía

## 🔧 Configuración Técnica

### Herramientas Requeridas
- **Sphinx** para generación de documentación
- **Autodoc** para extracción automática de docstrings
- **reStructuredText** como formato de marcado
- **Plantillas HTML** personalizadas
- **Integración con GitHub** para documentación online

### Estructura de Archivos
```
docs/
├── conf.py              # Configuración Sphinx
├── index.rst           # Página principal
├── overview/           # Visión general del sistema
├── api/               # Documentación API (autodoc)
├── user_guide/        # Guías de usuario
├── developer_guide/   # Guías de desarrollador
├── troubleshooting/   # Solución de problemas
└── _static/          # Recursos estáticos (imágenes, CSS)
```

## 📝 Estándares de Código

### Docstrings (Python/C++)
```cpp
/**
 * @brief Descripción breve de la funcionalidad
 * 
 * @details Descripción detallada de la implementación,
 * parámetros, valores de retorno y ejemplos de uso.
 * 
 * @param param1 Descripción del parámetro
 * @param param2 Descripción del segundo parámetro
 * @return Descripción del valor de retorno
 * @throws ExceptionType Descripción de excepciones
 * 
 * @example
 * ```cpp
 * // Ejemplo de uso
 * MyClass obj;
 * obj.method(param1, param2);
 * ```
 * 
 * @note Notas importantes sobre el comportamiento
 * @warning Advertencias sobre limitaciones
 * @see Referencia a otra clase/método
 */
```

### Comentarios de Clase
```cpp
/**
 * @class ClassName
 * @brief Descripción de la clase y su propósito
 * 
 * @details Descripción detallada de la funcionalidad,
 * responsabilidades, patrones de diseño utilizados
 * y ejemplos de uso.
 * 
 * Esta clase implementa [patrón/principio] para
 * [propósito específico]. Forma parte del módulo
 * [nombre del módulo] y colabora con [otras clases].
 * 
 * @example
 * ```cpp
 * // Ejemplo de creación y uso
 * ClassName obj;
 * obj.initialize();
 * obj.process();
 * ```
 * 
 * @author Nombre del autor
 * @version 1.0
 * @date Fecha de creación
 * @see RelatedClass
 * @see OtherRelatedClass
 */
```

## 🎨 Estilo y Presentación

### Características Visuales
- **Diseño profesional** y técnico
- **Navegación intuitiva** con menús jerárquicos
- **Código resaltado** con syntax highlighting
- **Diagramas técnicos** (UML, arquitectura, flujos)
- **Tablas organizadas** para comparaciones
- **Listas numeradas** para procedimientos

### Contenido Técnico
- **Explicaciones detalladas** de algoritmos
- **Ejemplos prácticos** de implementación
- **Casos de uso** reales del sistema
- **Optimizaciones** y mejores prácticas
- **Consideraciones** de rendimiento y memoria

## 📖 Secciones Específicas

### 1. Resumen Ejecutivo
- **Descripción del sistema** en términos técnicos
- **Características avanzadas** destacadas
- **Arquitectura general** con diagramas
- **Beneficios técnicos** del diseño modular

### 2. API Documentation (Autodoc)
- **Todas las clases** documentadas automáticamente
- **Métodos públicos** con ejemplos
- **Parámetros y tipos** detallados
- **Valores de retorno** especificados
- **Excepciones** documentadas

### 3. Guías de Usuario
- **Configuración inicial** paso a paso
- **Ejemplos de uso** prácticos
- **Configuración de sensores** detallada
- **Monitoreo y mantenimiento**

### 4. Guías de Desarrollador
- **Arquitectura del código** explicada
- **Patrones de diseño** utilizados
- **Extensibilidad** del sistema
- **Testing** y debugging

### 5. Troubleshooting
- **Problemas comunes** y soluciones
- **Diagnóstico** de errores
- **Logs** y debugging
- **Optimización** de rendimiento

## 🚀 Implementación

### Pasos de Ejecución
1. **Configurar Sphinx** con autodoc
2. **Estructurar archivos** .rst
3. **Generar documentación** automática
4. **Revisar y refinar** contenido
5. **Publicar** en plataforma web

### Comandos Típicos
```bash
# Configurar Sphinx
sphinx-quickstart docs

# Generar documentación
cd docs
make html

# Verificar enlaces
make linkcheck
```

## 📋 Checklist de Calidad

### Contenido
- [ ] **Docstrings completos** en todas las clases
- [ ] **Ejemplos de código** ejecutables
- [ ] **Diagramas técnicos** actualizados
- [ ] **Referencias cruzadas** funcionando
- [ ] **Navegación** intuitiva

### Técnico
- [ ] **Autodoc configurado** correctamente
- [ ] **Syntax highlighting** funcionando
- [ ] **Enlaces internos** verificados
- [ ] **Responsive design** implementado
- [ ] **Búsqueda** funcional

### Profesional
- [ ] **Tono técnico** apropiado
- [ ] **Precisión** en detalles técnicos
- [ ] **Consistencia** en terminología
- [ ] **Completitud** de información
- [ ] **Actualización** de versiones

## 🎯 Resultado Esperado

Una documentación técnica profesional que:
- **Explica claramente** la arquitectura del sistema
- **Proporciona ejemplos** prácticos de uso
- **Facilita el desarrollo** y mantenimiento
- **Sirve como referencia** técnica completa
- **Demuestra la calidad** del código y diseño

---

**Nota:** Este prompt debe ser utilizado como referencia completa para cualquier sesión de documentación del proyecto, asegurando consistencia y calidad técnica en toda la documentación generada. 