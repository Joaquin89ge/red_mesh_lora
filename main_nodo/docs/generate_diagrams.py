#!/usr/bin/env python3
"""
Script para generar diagramas profesionales para la documentación del proyecto.
Crea diagramas de arquitectura, flujo de datos, y componentes del sistema.
"""

import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.patches import FancyBboxPatch, Circle, Rectangle
import numpy as np
from PIL import Image, ImageDraw, ImageFont
import os


def create_architecture_diagram():
    """Crea un diagrama de arquitectura del sistema más detallado y atractivo"""
    fig, ax = plt.subplots(1, 1, figsize=(16, 12))
    ax.set_xlim(0, 16)
    ax.set_ylim(0, 12)
    ax.axis('off')

    # Colores profesionales
    colors = {
        'primary': '#2E86AB',
        'secondary': '#A23B72',
        'accent': '#F18F01',
        'success': '#C73E1D',
        'info': '#6C5B7B',
        'warning': '#F7931E',
        'light': '#E8F4FD',
        'dark': '#1A1A1A'
    }

    # Título
    ax.text(8, 11.5, 'SISTEMA DE MONITOREO AGRÍCOLA - ARQUITECTURA COMPLETA',
            fontsize=20, fontweight='bold', ha='center', color=colors['dark'])

    # Nodo de Campo (ESP32)
    node_box = FancyBboxPatch((1, 7), 6, 3, boxstyle="round,pad=0.1",
                              facecolor=colors['primary'], alpha=0.9, edgecolor=colors['dark'], linewidth=2)
    ax.add_patch(node_box)
    ax.text(4, 8.5, 'NODO DE CAMPO\nESP32', fontsize=14, fontweight='bold',
            ha='center', va='center', color='white')

    # Sensores
    sensors = [
        ('🌡️ DHT', 0.5, 5.5, colors['secondary']),
        ('🌱 NPK/pH/EC', 2.5, 5.5, colors['accent']),
        ('📍 GPS', 4.5, 5.5, colors['success']),
        ('⚡ VoltageReader', 6.5, 5.5, colors['info'])
    ]

    for name, x, y, color in sensors:
        sensor_circle = Circle((x, y), 0.4, facecolor=color,
                               alpha=0.8, edgecolor=colors['dark'], linewidth=1)
        ax.add_patch(sensor_circle)
        ax.text(x, y, name, fontsize=10, fontweight='bold',
                ha='center', va='center', color='white')

    # Gateway Central
    gateway_box = FancyBboxPatch((9, 7), 6, 3, boxstyle="round,pad=0.1",
                                 facecolor=colors['secondary'], alpha=0.9, edgecolor=colors['dark'], linewidth=2)
    ax.add_patch(gateway_box)
    ax.text(12, 8.5, 'GATEWAY CENTRAL\nESP32', fontsize=14, fontweight='bold',
            ha='center', va='center', color='white')

    # Comunicación LoRa
    lora_arrow = patches.FancyArrowPatch((7, 8.5), (9, 8.5),
                                         connectionstyle="arc3", color=colors['accent'],
                                         linewidth=3, arrowstyle='<->', mutation_scale=20)
    ax.add_patch(lora_arrow)
    ax.text(8, 9.2, 'LoRa Mesh\n433 MHz', fontsize=10, fontweight='bold',
            ha='center', va='center', color=colors['accent'])

    # Internet y Dashboard
    internet_box = FancyBboxPatch((9, 3), 6, 2, boxstyle="round,pad=0.1",
                                  facecolor=colors['success'], alpha=0.9, edgecolor=colors['dark'], linewidth=2)
    ax.add_patch(internet_box)
    ax.text(12, 4, 'INTERNET\nDASHBOARD WEB', fontsize=12, fontweight='bold',
            ha='center', va='center', color='white')

    # Conexión Gateway-Internet
    internet_arrow = patches.FancyArrowPatch((12, 7), (12, 5),
                                             connectionstyle="arc3", color=colors['success'],
                                             linewidth=2, arrowstyle='->', mutation_scale=15)
    ax.add_patch(internet_arrow)

    # Especificaciones técnicas
    specs = [
        '📡 LoRa: 433 MHz, 20 dBm, 10km alcance',
        '🌡️ Sensores: -40°C a +80°C, ±0.5°C precisión',
        '🌱 Suelo: NPK, pH 3-9, EC 0-5000 μS/cm',
        '⚡ Energía: 7 días autonomía, 12V/7Ah',
        '📊 Procesamiento: ESP32 Dual-Core 240MHz',
        '🔋 Consumo: 120mA activo, 5mA sleep'
    ]

    for i, spec in enumerate(specs):
        ax.text(0.5, 3.5 - i*0.4, spec, fontsize=9, color=colors['dark'])

    # Leyenda de colores
    legend_elements = [
        patches.Patch(color=colors['primary'], label='Nodo de Campo'),
        patches.Patch(color=colors['secondary'], label='Gateway Central'),
        patches.Patch(color=colors['success'], label='Internet/Dashboard'),
        patches.Patch(color=colors['accent'], label='Sensores'),
        patches.Patch(color=colors['info'], label='Comunicación')
    ]
    ax.legend(handles=legend_elements, loc='upper right',
              bbox_to_anchor=(0.98, 0.98))

    plt.tight_layout()
    plt.savefig('_static/architecture_diagram.png',
                dpi=300, bbox_inches='tight')
    plt.close()


def create_data_flow_diagram():
    """Crea un diagrama de flujo de datos más detallado"""
    fig, ax = plt.subplots(1, 1, figsize=(16, 10))
    ax.set_xlim(0, 16)
    ax.set_ylim(0, 10)
    ax.axis('off')

    colors = {
        'primary': '#2E86AB',
        'secondary': '#A23B72',
        'accent': '#F18F01',
        'success': '#C73E1D',
        'info': '#6C5B7B',
        'warning': '#F7931E',
        'light': '#E8F4FD',
        'dark': '#1A1A1A'
    }

    # Título
    ax.text(8, 9.5, 'FLUJO DE DATOS - SISTEMA DE MONITOREO AGRÍCOLA',
            fontsize=18, fontweight='bold', ha='center', color=colors['dark'])

    # Etapas del flujo
    stages = [
        ('SENSORES\nFÍSICOS', 1, 7.5, colors['primary']),
        ('ADC\nPROCESAMIENTO', 4, 7.5, colors['secondary']),
        ('FILTRADO\nCALIBRACIÓN', 7, 7.5, colors['accent']),
        ('ALMACENAMIENTO\nBUFFER', 10, 7.5, colors['success']),
        ('TRANSMISIÓN\nLoRa', 13, 7.5, colors['info'])
    ]

    for i, (name, x, y, color) in enumerate(stages):
        stage_box = FancyBboxPatch((x-1, y-0.8), 2, 1.6, boxstyle="round,pad=0.1",
                                   facecolor=color, alpha=0.9, edgecolor=colors['dark'], linewidth=2)
        ax.add_patch(stage_box)
        ax.text(x, y, name, fontsize=10, fontweight='bold',
                ha='center', va='center', color='white')

        # Flechas entre etapas
        if i < len(stages) - 1:
            arrow = patches.FancyArrowPatch((x+1, y), (stages[i+1][1]-1, y),
                                            connectionstyle="arc3", color=colors['dark'],
                                            linewidth=2, arrowstyle='->', mutation_scale=15)
            ax.add_patch(arrow)

    # Detalles de cada etapa
    details = [
        ['🌡️ DHT (Temp/Hum)', '🌱 NPK/pH/EC',
         '📍 GPS NEO-6M', '⚡ VoltageReader'],
        ['ADC 12-bit', 'Resolución 0-4095', 'Atenuación 11dB', 'Conversión A/D'],
        ['Media móvil', 'Calibración offset',
            'Validación rangos', 'Filtrado ruido'],
        ['Buffer circular', '8 muestras atm.', 'Datos suelo/GPS', 'Timestamp'],
        ['LoRa 433MHz', 'Mesh routing', 'ACK/Retry', 'Gateway']
    ]

    for i, detail_list in enumerate(details):
        y_pos = 5.5 - i * 0.8
        for j, detail in enumerate(detail_list):
            x_pos = 1 + j * 3.5
            ax.text(x_pos, y_pos, detail, fontsize=8, ha='center', va='center',
                    color=colors['dark'], bbox=dict(boxstyle="round,pad=0.2",
                                                    facecolor=colors['light'], alpha=0.7))

    # Especificaciones técnicas
    tech_specs = [
        '📊 Frecuencia de muestreo: 1 Hz',
        '🔧 Resolución ADC: 12 bits (0-4095)',
        '⚡ Tiempo de respuesta: <2 segundos',
        '📡 Tasa de transmisión: 0.3-37.5 kbps',
        '🔋 Consumo energético: 120mA activo',
        '🌐 Alcance de comunicación: 10 km'
    ]

    for i, spec in enumerate(tech_specs):
        ax.text(0.5, 2.5 - i*0.3, spec, fontsize=9, color=colors['dark'])

    plt.tight_layout()
    plt.savefig('_static/data_flow_diagram.png', dpi=300, bbox_inches='tight')
    plt.close()


def create_voltage_reader_diagram():
    """Crea un diagrama específico del VoltageReader"""
    fig, ax = plt.subplots(1, 1, figsize=(14, 10))
    ax.set_xlim(0, 14)
    ax.set_ylim(0, 10)
    ax.axis('off')

    colors = {
        'primary': '#2E86AB',
        'secondary': '#A23B72',
        'accent': '#F18F01',
        'success': '#C73E1D',
        'info': '#6C5B7B',
        'warning': '#F7931E',
        'light': '#E8F4FD',
        'dark': '#1A1A1A'
    }

    # Título
    ax.text(7, 9.5, 'VOLTAGE READER - DIVISOR RESISTIVO ESP32',
            fontsize=16, fontweight='bold', ha='center', color=colors['dark'])

    # Diagrama del circuito
    # Entrada de voltaje
    input_box = FancyBboxPatch((1, 7), 2, 1.5, boxstyle="round,pad=0.1",
                               facecolor=colors['primary'], alpha=0.9, edgecolor=colors['dark'], linewidth=2)
    ax.add_patch(input_box)
    ax.text(2, 7.75, 'ENTRADA\n0-15V', fontsize=10, fontweight='bold',
            ha='center', va='center', color='white')

    # Divisor resistivo
    divider_box = FancyBboxPatch((4, 6.5), 3, 2.5, boxstyle="round,pad=0.1",
                                 facecolor=colors['secondary'], alpha=0.9, edgecolor=colors['dark'], linewidth=2)
    ax.add_patch(divider_box)
    ax.text(5.5, 7.75, 'DIVISOR\nRESISTIVO\nR1 + R2', fontsize=10,
            fontweight='bold', ha='center', va='center', color='white')

    # ADC ESP32
    adc_box = FancyBboxPatch((8, 7), 2, 1.5, boxstyle="round,pad=0.1",
                             facecolor=colors['accent'], alpha=0.9, edgecolor=colors['dark'], linewidth=2)
    ax.add_patch(adc_box)
    ax.text(9, 7.75, 'ADC ESP32\nPin 34', fontsize=10,
            fontweight='bold', ha='center', va='center', color='white')

    # Procesamiento
    proc_box = FancyBboxPatch((11, 7), 2, 1.5, boxstyle="round,pad=0.1",
                              facecolor=colors['success'], alpha=0.9, edgecolor=colors['dark'], linewidth=2)
    ax.add_patch(proc_box)
    ax.text(12, 7.75, 'PROCESAMIENTO\nVoltageReader', fontsize=10,
            fontweight='bold', ha='center', va='center', color='white')

    # Flechas
    arrows = [
        ((3, 7.75), (4, 7.75)),
        ((7, 7.75), (8, 7.75)),
        ((10, 7.75), (11, 7.75))
    ]

    for start, end in arrows:
        arrow = patches.FancyArrowPatch(start, end, connectionstyle="arc3",
                                        color=colors['dark'], linewidth=2,
                                        arrowstyle='->', mutation_scale=15)
        ax.add_patch(arrow)

    # Especificaciones técnicas
    specs = [
        '⚡ Rango de entrada: 0-15V',
        '🔧 Rango de salida: 0-2.5V',
        '📊 Resolución ADC: 12 bits (0-4095)',
        '🎯 Precisión: ±0.1V',
        '🔍 Filtrado: Media móvil 8 muestras',
        '⚙️ Calibración: Offset y pendiente',
        '📈 Mapeo: map() de Arduino optimizado'
    ]

    for i, spec in enumerate(specs):
        ax.text(0.5, 5.5 - i*0.4, spec, fontsize=9, color=colors['dark'])

    # Código de ejemplo
    code_example = [
        'VoltageReader voltageReader;',
        'voltageReader.begin();',
        'voltageReader.calibrate(0.0, 2.5);',
        'float voltage = voltageReader.readVoltage();',
        'float percentage = voltageReader.readVoltagePercentage();',
        'int mapped = voltageReader.readVoltageMappedInt(0, 100);'
    ]

    for i, line in enumerate(code_example):
        ax.text(7, 4.5 - i*0.3, line, fontsize=8, color=colors['dark'],
                family='monospace', bbox=dict(boxstyle="round,pad=0.2",
                                              facecolor=colors['light'], alpha=0.7))

    plt.tight_layout()
    plt.savefig('_static/voltage_reader_diagram.png',
                dpi=300, bbox_inches='tight')
    plt.close()


def create_logo():
    """Crea un logo profesional para el proyecto"""
    # Crear imagen
    width, height = 400, 200
    img = Image.new('RGB', (width, height), color='#2E86AB')
    draw = ImageDraw.Draw(img)

    try:
        # Intentar usar una fuente del sistema
        font_large = ImageFont.truetype("arial.ttf", 32)
        font_small = ImageFont.truetype("arial.ttf", 16)
    except:
        # Fallback a fuente por defecto
        font_large = ImageFont.load_default()
        font_small = ImageFont.load_default()

    # Dibujar elementos del logo
    # Círculo principal (representando un nodo)
    draw.ellipse([50, 50, 150, 150], fill='#A23B72', outline='white', width=3)

    # Antena LoRa
    draw.line([100, 30, 100, 50], fill='#F18F01', width=4)
    draw.line([90, 30, 110, 30], fill='#F18F01', width=4)

    # Sensores
    sensor_positions = [(180, 80), (220, 80), (200, 120)]
    sensor_colors = ['#C73E1D', '#6C5B7B', '#F7931E']
    sensor_symbols = ['🌡️', '🌱', '⚡']

    for i, (x, y) in enumerate(sensor_positions):
        draw.ellipse([x-15, y-15, x+15, y+15],
                     fill=sensor_colors[i], outline='white', width=2)
        # Nota: Los emojis pueden no renderizarse correctamente en todas las fuentes
        draw.text((x-8, y-8), sensor_symbols[i], fill='white', font=font_small)

    # Texto del logo
    draw.text((200, 60), 'SISTEMA DE', fill='white', font=font_large)
    draw.text((200, 90), 'MONITOREO', fill='white', font=font_large)
    draw.text((200, 120), 'AGRÍCOLA', fill='white', font=font_large)
    draw.text((200, 150), 'IoT LoRa Mesh', fill='#F18F01', font=font_small)

    # Guardar logo
    img.save('_static/logo.png')


def main():
    """Función principal para generar todos los diagramas"""
    print("🎨 Generando diagramas profesionales...")

    # Crear directorio _static si no existe
    os.makedirs('_static', exist_ok=True)

    # Generar diagramas
    print("📊 Generando diagrama de arquitectura...")
    create_architecture_diagram()

    print("🔄 Generando diagrama de flujo de datos...")
    create_data_flow_diagram()

    print("⚡ Generando diagrama del VoltageReader...")
    create_voltage_reader_diagram()

    print("🎯 Generando logo del proyecto...")
    create_logo()

    print("✅ Todos los diagramas generados exitosamente en _static/")
    print("📁 Archivos creados:")
    print("   - architecture_diagram.png")
    print("   - data_flow_diagram.png")
    print("   - voltage_reader_diagram.png")
    print("   - logo.png")


if __name__ == "__main__":
    main()
