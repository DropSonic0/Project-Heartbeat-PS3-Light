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

    uint64_t file_base = 0;
    if (pck_version == 2) {
        f->get_32_le(); // flags (4 bytes)
        file_base = f->get_64_le(); // file_base (8 bytes)
    }
    UtilityFunctions::print("PCK: File Base: " + String::num_int64(file_base));

    // Reserved (16 * 4 = 64 bytes)
    for (int i=0; i<16; i++) f->get_32_le();

    uint32_t file_count = f->get_32_le();
    UtilityFunctions::print("PCK: File Count: " + String::num(file_count));
    
    for (uint32_t i = 0; i < file_count; i++) {
        uint32_t path_len = f->get_32_le();
        if (path_len > 1024) {
            UtilityFunctions::print("PCK: Error: path_len too long: " + String::num(path_len));
            return false;
        }
        PackedByteArray path_buf = f->get_buffer(path_len);
        
        // Path is padded to 4 bytes
        uint32_t padding = (4 - (path_len % 4)) % 4;
        for (uint32_t j = 0; j < padding; j++) {
            f->get_8();
        }

        String path_str = "";
        for(uint32_t j=0; j<path_len; j++) {
            char c = (char)path_buf[j];
            if (c == 0) continue;
            if (c == '\\') c = '/'; // Normalize slashes
            path_str += c;
        }
        if (!path_str.begins_with("res://")) {
            if (path_str.begins_with("/")) {
                path_str = "res://" + path_str.substr(1);
            } else {
                path_str = "res://" + path_str;
            }
        }
        
        uint64_t offset = f->get_64_le();
        uint64_t size = f->get_64_le();
        
        // Skip MD5 (16 bytes)
        f->get_buffer(16);

        uint32_t flags = 0;
        // Check if there are flags (Version 2)
        if (pck_version >= 2) {
            flags = f->get_32_le(); // flags
        }
        
        String path_lower_diag = path_str.to_lower();
        if (i < 50 || path_str.find("song.json") != std::string::npos || path_lower_diag.find("logo") != std::string::npos || path_lower_diag.find("icon") != std::string::npos || path_lower_diag.find("graphics") != std::string::npos) {
            UtilityFunctions::print("PCK: File " + String::num(i) + ": " + path_str + " (Size: " + String::num(size) + ", Flags: " + String::num(flags) + ", Offset: " + String::num_int64(offset) + " Base: " + String::num_int64(file_base) + ")");
            if (flags != 0) {
                UtilityFunctions::print("PCK: WARNING: File has FLAGS: " + String::num(flags) + " (Encrypted/Compressed?)");
            }
        }

        files[path_str] = {offset + file_base, size, flags};
        lowercase_files[path_str.to_lower()] = path_str;
        // UtilityFunctions::print("PCK: Found file: " + path_str);
    }

    pck_path = p_path;
    UtilityFunctions::print("PCKReader: Loaded " + p_path + " with " + String::num(files.size()) + " files.");
    return true;
}

bool PCKReader::file_exists(const String& p_path) const {
    if (files.find(p_path) != files.end()) return true;
    return lowercase_files.find(p_path.to_lower()) != lowercase_files.end();
}

PCKFileEntry PCKReader::get_file_entry(const String& p_path) const {
    auto it = files.find(p_path);
    if (it != files.end()) return it->second;

    auto it_lower = lowercase_files.find(p_path.to_lower());
    if (it_lower != lowercase_files.end()) {
        auto it_orig = files.find(it_lower->second);
        if (it_orig != files.end()) return it_orig->second;
    }

    return {0, 0, 0};
}

}
