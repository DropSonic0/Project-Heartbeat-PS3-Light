#ifndef HB_SHIFT_JIS_TABLE_HPP
#define HB_SHIFT_JIS_TABLE_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/object.hpp"
#else
#include <godot_cpp/classes/object.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/variant/packed_byte_array.hpp"
#else
#include <godot_cpp/variant/packed_byte_array.hpp>
#endif

namespace godot {

class ShiftJISTableNative : public Object {
    GDCLASS(ShiftJISTableNative, Object);

protected:
    static void _bind_methods();

public:
    static PackedByteArray get_conv_table();
};

} // namespace godot

#endif // HB_SHIFT_JIS_TABLE_HPP
