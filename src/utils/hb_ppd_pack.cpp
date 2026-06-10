#include "hb_ppd_pack.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void PPDPackNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_file_index", "file_name"), &PPDPackNative::get_file_index);
}

PPDPackNative::PPDPackNative() {}
PPDPackNative::~PPDPackNative() {}

int PPDPackNative::get_file_index(const String &p_file_name) {
    return -1;
}
