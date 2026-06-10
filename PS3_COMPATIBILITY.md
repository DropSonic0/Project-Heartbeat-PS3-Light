# Notas de Compatibilidad: VS2012 y PS3 SDK

El objetivo de este proyecto es compilar para PS3 usando Visual Studio 2012. Esto impone restricciones significativas en el código C++:

## 1. Estándar de C++
VS2012 tiene un soporte limitado para C++11. Evita usar características modernas de C++14, C++17 o C++20.
- **Usa**: `NULL` o `0` si `nullptr` da problemas (aunque VS2012 lo soporta parcialmente).
- **Evita**: `auto` (excepto en casos simples), `constexpr`, inicialización de miembros en la declaración (algunas versiones de VS2012 fallan), lambdas complejas.

## 2. GDExtension y godot-cpp
La biblioteca `godot-cpp` estándar asume un compilador moderno (C++17). Para compilar para PS3/VS2012:
1.  Tendrás que modificar `godot-cpp` para que sea compatible con C++11 o inferior.
2.  Es probable que necesites usar una versión más antigua de Godot (como la 3.x con GDNative) o hacer un port manual de las cabeceras de GDExtension.

## 3. Tipos de Datos
Asegúrate de usar los tipos de Godot (`int64_t`, `double`, `String`) que son más fáciles de portar que los tipos estándar de la STL si el SDK de PS3 tiene una implementación de STL incompleta.

## 4. Estructura de Archivos
Todos los archivos de lógica ahora están en `src/` en formato `.cpp` y `.hpp`, lo que facilita su inclusión en un proyecto de VS2012. Solo tendrás que añadir los archivos al `.vcxproj` generado.

## 5. Uso del Shim de Compatibilidad
Para compilar sin las librerías oficiales de Godot en VS2012:
1. Define `PS3_SDK` en tu preprocesador.
2. Añade `src/compat` a tus rutas de inclusión.
3. El código usará las clases ligeras definidas en `src/compat/godot_cpp` que emulan el comportamiento de Godot usando C++ estándar compatible con VS2012.
