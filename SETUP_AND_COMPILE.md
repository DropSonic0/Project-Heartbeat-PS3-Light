# Guía de Configuración y Compilación (GDExtension)

Para compilar este proyecto en C++, necesitas configurar el entorno de `godot-cpp`.

## 1. Requisitos de Software

*   **Python 3.6+**: Necesario para ejecutar SCons.
*   **SCons 3.0+**: El sistema de construcción. Instalación: `pip install scons`
*   **Compilador de C++**:
    *   **Windows**: Visual Studio 2022 (Recomendado para Godot 4) o MinGW-w64.
    *   **Linux**: GCC o Clang.
    *   **macOS**: Xcode.

## 2. Configuración de `godot-cpp`

Este repositorio usa GDExtension, lo que requiere las librerías de enlace de Godot.

1.  Abre una terminal en la raíz del proyecto.
2.  Clona el repositorio `godot-cpp`:
    ```bash
    git clone -b 4.2 https://github.com/godotengine/godot-cpp.git
    ```
    *(Nota: Usa la rama que coincida con tu versión de Godot. Para este proyecto usamos 4.2)*.
3.  Entra en la carpeta y genera las librerías de enlace (bindings):
    ```bash
    cd godot-cpp
    scons platform=<tu_plataforma> target=template_debug
    cd ..
    ```
    *Reemplaza `<tu_plataforma>` por `windows`, `linux` o `macos`.*

## 3. Compilación del Proyecto

Una vez que `godot-cpp` está listo, puedes compilar el código de Project Heartbeat:

```bash
# Para Windows
scons platform=windows target=template_debug

# Para Linux
scons platform=linux target=template_debug
```

Los binarios resultantes se guardarán en `addons/project_heartbeat/bin/`.

## 4. Notas para PS3 (VS2012)

Si intentas compilar esto para PS3 con VS2012:
1.  **NO** uses la versión oficial de `godot-cpp` sin modificar, ya que requiere C++17.
2.  Tendrás que crear un proyecto de Visual Studio 2012 manualmente y añadir los archivos de `src/`.
3.  Consulta `PS3_COMPATIBILITY.md` para más detalles sobre las limitaciones del lenguaje.
