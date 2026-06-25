#include "hb_user_settings.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBUserSettingsNative::_bind_methods() {}

HBUserSettingsNative::HBUserSettingsNative() {
    root_folder.instantiate();

    set("visualizer_enabled", true);
    set("visualizer_resolution", 32);
    set("ui_blur_enabled", false);
    set("lag_compensation", 0);
    set("note_size", 1.0);
    set("icon_pack", String("playstation"));
    set("resource_pack", String("playstation"));
    set("fps_limit", 180);
    set("display_mode", String("borderless"));
    set("display", -1);

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
