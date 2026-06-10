#include "hb_shift_jis_table.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ShiftJISTableNative::_bind_methods() {
    ClassDB::bind_static_method("ShiftJISTableNative", D_METHOD("get_conv_table"), &ShiftJISTableNative::get_conv_table);
}

PackedByteArray ShiftJISTableNative::get_conv_table() {
    PackedByteArray table;
    // The table would be hardcoded here or loaded from a file
    return table;
}
