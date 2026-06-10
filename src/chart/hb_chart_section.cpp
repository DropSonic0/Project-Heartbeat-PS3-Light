#include "hb_chart_section.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void HBChartSectionNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_section_name", "name"), &HBChartSectionNative::set_section_name);
    ClassDB::bind_method(D_METHOD("get_section_name"), &HBChartSectionNative::get_section_name);
    ClassDB::bind_method(D_METHOD("set_color", "color"), &HBChartSectionNative::set_color);
    ClassDB::bind_method(D_METHOD("get_color"), &HBChartSectionNative::get_color);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_section_name", "get_section_name");
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
}

HBChartSectionNative::HBChartSectionNative() {
    serializable_fields.append("name");
    serializable_fields.append("color");

    static const char* palette[] = {
        "#81a1c1", "#bf616a", "#d08770", "#ebcb8b", "#a3be8c", "#b48ead", "#d8dee9"
    };
    int idx = UtilityFunctions::randi() % 7;
    color = Color(palette[idx]);
}

HBChartSectionNative::~HBChartSectionNative() {}

void HBChartSectionNative::set_section_name(String p_name) { name = p_name; }
String HBChartSectionNative::get_section_name() const { return name; }
void HBChartSectionNative::set_color(Color p_color) { color = p_color; }
Color HBChartSectionNative::get_color() const { return color; }

String HBChartSectionNative::get_serialized_type() const {
    return "ChartSection";
}
