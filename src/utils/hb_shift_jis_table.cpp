#include "hb_shift_jis_table.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ShiftJISTableNative::_bind_methods() {
    ClassDB::bind_static_method("ShiftJISTableNative", D_METHOD("get_conv_table"), &ShiftJISTableNative::get_conv_table);
}

#include "hb_shift_jis_table_data.ipp"

PackedByteArray ShiftJISTableNative::get_conv_table() {
    PackedByteArray table;
    table.resize(sizeof(sjis_table_data));
    for (size_t i = 0; i < sizeof(sjis_table_data); i++) {
        table[i] = sjis_table_data[i];
    }
    return table;
}
