#ifndef HB_SHIFT_JIS_TABLE_HPP
#define HB_SHIFT_JIS_TABLE_HPP

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

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
