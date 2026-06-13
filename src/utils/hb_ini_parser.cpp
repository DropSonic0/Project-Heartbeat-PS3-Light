#include "hb_ini_parser.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBINIParserNative::_bind_methods() {
    ClassDB::bind_static_method("HBINIParserNative", D_METHOD("parse", "contents"), &HBINIParserNative::parse);
}

Dictionary HBINIParserNative::parse(const String &p_contents) {
    Dictionary dict;
    String current_section = "";
    PackedStringArray lines = p_contents.split("\n");
    for (int i = 0; i < lines.size(); i++) {
        String line = lines[i].strip_edges();
        if (line.begins_with("[") && line.ends_with("]")) {
            current_section = line.substr(1, line.length() - 2);
            dict[current_section] = Dictionary();
        } else if (!line.is_empty()) {
            PackedStringArray vk_pair = line.split("=");
            if (vk_pair.size() > 1) {
                if (!current_section.is_empty()) {
                    Dictionary section_dict = dict[current_section];
                    section_dict[((String)vk_pair[0]).strip_edges()] = ((String)vk_pair[1]).strip_edges();
                }
            } else if (vk_pair.size() == 1) {
                if (!current_section.is_empty()) {
                    Dictionary section_dict = dict[current_section];
                    section_dict[((String)vk_pair[0]).strip_edges()] = Variant();
                }
            }
        }
    }
    return dict;
}
