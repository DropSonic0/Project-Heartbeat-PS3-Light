#include "hb_editor_template.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBEditorTemplateNative::_bind_methods() {}

HBEditorTemplateNative::HBEditorTemplateNative() {
    serializable_fields.append("name");
    serializable_fields.append("saved_properties");
    serializable_fields.append("autohide");
    serializable_fields.append("up_template");
    serializable_fields.append("down_template");
    serializable_fields.append("left_template");
    serializable_fields.append("right_template");
    serializable_fields.append("slide_left_template");
    serializable_fields.append("slide_right_template");
    serializable_fields.append("slide_chain_left_template");
    serializable_fields.append("slide_chain_right_template");
    serializable_fields.append("slide_left_2_template");
    serializable_fields.append("slide_right_2_template");
    serializable_fields.append("slide_chain_left_2_template");
    serializable_fields.append("slide_chain_right_2_template");
    serializable_fields.append("heart_template");
}

HBEditorTemplateNative::~HBEditorTemplateNative() {}
