#!/usr/bin/env python3
"""
Script para generar la documentación del Sistema de Monitoreo Agrícola
"""

import os
import sys
import subprocess
import shutil


def main():
    print("Generando documentación del Sistema de Monitoreo Agrícola...")

    # Verificar que estamos en el directorio correcto
    if not os.path.exists('conf.py'):
        print("Error: No se encontró conf.py. Ejecutar desde el directorio docs/")
        return 1

    # Crear directorio de build si no existe
    if not os.path.exists('_build'):
        os.makedirs('_build')

    # Comando para generar HTML
    cmd = [
        sys.executable, '-m', 'sphinx.cmd.build',
        '-b', 'html',
        '.', '_build/html'
    ]

    try:
        print("Ejecutando sphinx-build...")
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode == 0:
            print("✅ Documentación generada exitosamente en _build/html/")
            print("📁 Archivos generados:")

            # Listar archivos generados
            html_dir = '_build/html'
            if os.path.exists(html_dir):
                for root, dirs, files in os.walk(html_dir):
                    for file in files:
                        if file.endswith('.html'):
                            rel_path = os.path.relpath(
                                os.path.join(root, file), html_dir)
                            print(f"   - {rel_path}")

            print(
                "\n🌐 Para ver la documentación, abre _build/html/index.html en tu navegador")
            return 0
        else:
            print("❌ Error al generar la documentación:")
            print(result.stderr)
            return 1

    except Exception as e:
        print(f"❌ Error: {e}")
        return 1


if __name__ == '__main__':
    sys.exit(main())
