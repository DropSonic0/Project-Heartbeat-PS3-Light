#include "hb_ui_component.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBUIComponentNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_color_string", "color"), &HBUIComponentNative::get_color_string);
    ClassDB::bind_method(D_METHOD("get_color_from_dict", "dict", "key", "backup"), &HBUIComponentNative::get_color_from_dict);
}

HBUIComponentNative::HBUIComponentNative() {}
HBUIComponentNative::~HBUIComponentNative() {}

void HBUIComponentNative::_ready() {
    add_to_group(get_component_id());
}

String HBUIComponentNative::get_color_string(Color p_color) const {
    String col = p_color.to_html();
    String alpha = col.substr(col.length() - 2);
    return alpha + col.substr(0, col.length() - 2);
}

Color HBUIComponentNative::get_color_from_dict(Dictionary p_dict, String p_key, Color p_backup) const {
    if (p_dict.has(p_key)) {
        String col_str = p_dict[p_key];
        if (!col_str.is_empty()) {
            // Convert ARGB back to RGBA for Godot Color
            String rgba = col_str.substr(2) + col_str.substr(0, 2);
            return Color(rgba);
        }
    }
    return p_backup;
}
