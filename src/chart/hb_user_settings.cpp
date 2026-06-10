#include "hb_user_settings.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBUserSettingsNative::_bind_methods() {}

HBUserSettingsNative::HBUserSettingsNative() {
    root_folder.instantiate();

    serializable_fields.append("visualizer_enabled");
    serializable_fields.append("visualizer_resolution");
    serializable_fields.append("ui_blur_enabled");
    serializable_fields.append("lag_compensation");
    serializable_fields.append("note_size");
    serializable_fields.append("icon_pack");
    serializable_fields.append("resource_pack");
    serializable_fields.append("ui_skin");
    serializable_fields.append("input_map");
    serializable_fields.append("fps_limit");
    serializable_fields.append("display_mode");
    serializable_fields.append("display");
    serializable_fields.append("root_folder");
}

HBUserSettingsNative::~HBUserSettingsNative() {}
