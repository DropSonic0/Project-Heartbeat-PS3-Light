#include "hb_toml_parser.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBTOMLParserNative::_bind_methods() {
    ClassDB::bind_static_method("HBTOMLParserNative", D_METHOD("parse", "contents"), &HBTOMLParserNative::parse);
    ClassDB::bind_static_method("HBTOMLParserNative", D_METHOD("parse_key", "key"), &HBTOMLParserNative::parse_key);
}

Array HBTOMLParserNative::parse_key(const String &p_key) {
    return p_key.split(".");
}

Dictionary HBTOMLParserNative::parse(const String &p_contents) {
    Dictionary out;
    out["default"] = Dictionary();

    Dictionary output_ref = out["default"];

    PackedStringArray lines = p_contents.split("\n");
    for (int i = 0; i < lines.size(); i++) {
        String line = lines[i].strip_edges();
        if (line.is_empty()) continue;

        if (line.begins_with("[[")) {
            String key = line.substr(2, line.length() - 4); // [[key]] -> key
            Array new_sections = parse_key(key);
            if (new_sections.is_empty()) continue;
            String array_key = new_sections.pop_back();

            output_ref = out;
            for (int j = 0; j < new_sections.size(); j++) {
                String section = new_sections[j];
                if (output_ref.has(section)) {
                    Variant new_ref = output_ref[section];
                    if (new_ref.get_type() == Variant::ARRAY) {
                        Array arr = new_ref;
                        if (!arr.is_empty()) {
                            output_ref = arr[arr.size() - 1];
                        }
                    } else {
                        output_ref = new_ref;
                    }
                } else {
                    Dictionary d;
                    output_ref[section] = d;
                    output_ref = d;
                }
            }

            if (!output_ref.has(array_key)) {
                output_ref[array_key] = Array();
            }
            Array arr = output_ref[array_key];
            Dictionary new_entry;
            arr.push_back(new_entry);
            output_ref = new_entry;
            continue;
        }

        if (line.begins_with("[")) {
            String key = line.substr(1, line.length() - 2);
            Array new_sections = parse_key(key);
            if (new_sections.is_empty()) continue;
            String table_key = new_sections.pop_back();

            output_ref = out;
            for (int j = 0; j < new_sections.size(); j++) {
                String section = new_sections[j];
                if (output_ref.has(section)) {
                    Variant new_ref = output_ref[section];
                    if (new_ref.get_type() == Variant::ARRAY) {
                        Array arr = new_ref;
                        output_ref = arr[arr.size() - 1];
                    } else {
                        output_ref = new_ref;
                    }
                } else {
                    Dictionary d;
                    output_ref[section] = d;
                    output_ref = d;
                }
            }
            Dictionary d;
            output_ref[table_key] = d;
            output_ref = d;
            continue;
        }

        PackedStringArray line_split = line.split("=");
        if (line_split.size() > 1) {
            String line_key = line_split[0].strip_edges();
            String line_value = line_split[1].strip_edges();

            Variant val;
            bool handled = false;

            if (line_value.begins_with("\"") && line_value.ends_with("\"")) {
                val = line_value.substr(1, line_value.length() - 2);
                handled = true;
            } else if (line_value == "true") {
                val = true;
                handled = true;
            } else if (line_value == "false") {
                val = false;
                handled = true;
            } else if (line_value.is_valid_int()) {
                val = line_value.to_int();
                handled = true;
            } else if (line_value.is_valid_float()) {
                val = line_value.to_float();
                handled = true;
            }

            if (handled) {
                output_ref[line_key] = val;
            }
        }
    }
    return out;
}
