# Guía de Migración a C++ para Project Heartbeat

Esta guía explica cómo migrar las clases de GDScript a las nuevas implementaciones nativas en C++.

## 1. Cambio de Herencia
Para que un archivo `.gd` use la lógica de C++, debes cambiar la clase que extiende.

**Antes:**
```gdscript
extends HBNoteData
class_name MiNotaPersonalizada
```

**Después:**
```gdscript
extends HBNoteDataNative
class_name MiNotaPersonalizada
```

## 2. Actualización de Propiedades
Las clases de C++ exponen las mismas propiedades, pero asegúrate de usar los nombres correctos. Por ejemplo:
- `HBTimingPointNative`: `time`
- `HBBaseNoteNative`: `position`, `distance`, `note_type`
- `HBNoteDataNative`: `hold`

## 3. Uso de Tipos en Arrays
Si tienes arrays de notas, ahora serán arrays de objetos de C++. La serialización se encarga de convertirlos automáticamente si usas `HBChartNative.deserialize()`.

## 4. Funciones Útiles
Usa `HBUtilsNative` en lugar de `HBUtils` para operaciones pesadas:
```gdscript
var pos = HBUtilsNative.calculate_note_sine(time, base_pos, angle, freq, amp, dist)
```

## 5. Compilación
Recuerda compilar siempre después de hacer cambios en el código de la carpeta `src/`:
```bash
scons platform=<plataforma> target=template_debug
```
