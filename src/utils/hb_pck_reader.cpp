#include "hb_pck_reader.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"
#include <string.h>

namespace godot {

PCKReader::PCKReader() {}

bool PCKReader::load_pck(const String& p_path) {
    Ref<FileAccess> f = FileAccess::open(p_path, FileAccess::READ);
    if (f.is_null()) return false;

    uint32_t magic = f->get_32(); // Magic is usually 4 bytes, "GDPC" in LE is 0x43504447
    char magic_str[5];
    memcpy(magic_str, &magic, 4);
    magic_str[4] = 0;
    UtilityFunctions::print("PCK: Magic: " + String(magic_str) + " (" + String::num_int64(magic) + ")");

    if (magic != 0x43504447 && magic != 0x47445043) { // Support both LE and BE check for safety
        UtilityFunctions::print("PCK: Invalid magic!");
        return false;
    }

    uint32_t pck_version = f->get_32_le();
    uint32_t ver_major = f->get_32_le();
    uint32_t ver_minor = f->get_32_le();
    uint32_t ver_rev = f->get_32_le();
    
    UtilityFunctions::print("PCK: Version: " + String::num(pck_version));
    UtilityFunctions::print("PCK: Godot Version: " + String::num(ver_major) + "." + String::num(ver_minor) + "." + String::num(ver_rev));

    uint64_t file_table_offset = 0;
    if (pck_version == 2) {
        f->get_32_le(); // flags
        file_table_offset = f->get_64_le();
    }

    // Reserved
    for (int i=0; i<16; i++) f->get_32_le();

    if (pck_version == 2 && file_table_offset != 0) {
        f->seek((size_t)file_table_offset);
    }

    uint32_t file_count = f->get_32_le();
    UtilityFunctions::print("PCK: File Count: " + String::num(file_count));
    
    // In Godot 4 (PCK v2), the file table follows the count.
    // If there's an offset specified in the header, we might need to seek there,
    // but usually it's just sequential after file_count.
    
    for (uint32_t i = 0; i < file_count; i++) {
        uint32_t path_len = f->get_32_le();
        PackedByteArray path_buf = f->get_buffer(path_len);
        
        String path_str = "res://";
        for(uint32_t j=0; j<path_len; j++) {
            if (path_buf[j] != 0) path_str += (char)path_buf[j];
        }
        
        if (i < 20 || !path_str.begins_with("res://.godot")) {
            UtilityFunctions::print("PCK: File " + String::num(i) + ": " + path_str);
        }

        uint64_t offset = f->get_64_le();
        uint64_t size = f->get_64_le();
        
        // Skip MD5 (16 bytes)
        f->get_buffer(16);

        // Check if there are flags (Version 2)
        if (pck_version >= 2) {
            f->get_32_le(); // flags
        }

        files[path_str] = {offset, size};
        // UtilityFunctions::print("PCK: Found file: " + path_str);
    }

    pck_path = p_path;
    UtilityFunctions::print("PCKReader: Loaded " + p_path + " with " + String::num(files.size()) + " files.");
    return true;
}

bool PCKReader::file_exists(const String& p_path) const {
    return files.find(p_path) != files.end();
}

PCKFileEntry PCKReader::get_file_entry(const String& p_path) const {
    auto it = files.find(p_path);
    if (it != files.end()) return it->second;
    return {0, 0};
}

}
