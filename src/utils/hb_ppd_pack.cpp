#include "hb_ppd_pack.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void PPDPackNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("load", "path"), &PPDPackNative::load);
    ClassDB::bind_method(D_METHOD("get_file_index", "file_name"), &PPDPackNative::get_file_index);
}

PPDPackNative::PPDPackNative() {}
PPDPackNative::~PPDPackNative() {}

bool PPDPackNative::load(const String &p_path) {
    file = FileAccess::open(p_path, FileAccess::READ);
    if (file.is_null()) {
        valid = false;
        return false;
    }

    String signature = "";
    PackedByteArray sig_buf = file->get_buffer(9);
    for (int i=0; i<9; i++) signature += (char)sig_buf[i];

    if (signature != "PPDPACKV1") {
        valid = false;
        return false;
    }

    std::vector<uint8_t> name_lengths;
    uint8_t len = file->get_8();
    while (len > 0) {
        name_lengths.push_back(len);
        len = file->get_8();
    }

    for (size_t i = 0; i < name_lengths.size(); i++) {
        PackedByteArray name_buf = file->get_buffer(name_lengths[i]);
        String name = "";
        for (int j=0; j<name_buf.size(); j++) name += (char)name_buf[j];
        file_names.push_back(name);
        
        uint32_t size = file->get_32();
        file_sizes.push_back(size);
    }

    size_t offset = file->get_position();
    for (size_t i = 0; i < file_sizes.size(); i++) {
        file_offsets.push_back(offset);
        offset += file_sizes[i];
    }

    return true;
}

int PPDPackNative::get_file_index(const String &p_file_name) {
    for (size_t i = 0; i < file_names.size(); i++) {
        if (file_names[i] == p_file_name) {
            return (int)i;
        }
    }
    return -1;
}
