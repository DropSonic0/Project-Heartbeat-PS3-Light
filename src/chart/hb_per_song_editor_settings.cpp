#include "hb_per_song_editor_settings.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBPerSongEditorSettingsNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_layer_visibility", "visibility", "layer_name"), &HBPerSongEditorSettingsNative::set_layer_visibility);
}

HBPerSongEditorSettingsNative::HBPerSongEditorSettingsNative() {
    serializable_fields.append("hidden_layers");
    serializable_fields.append("note_resolution");
    serializable_fields.append("timeline_snap");
    serializable_fields.append("show_video");
    serializable_fields.append("show_bg");
    serializable_fields.append("selected_variant");

    // Default hidden layers
    hidden_layers.append("SlideRight2");
    hidden_layers.append("SlideLeft2");
    hidden_layers.append("Heart");
}

HBPerSongEditorSettingsNative::~HBPerSongEditorSettingsNative() {}

void HBPerSongEditorSettingsNative::set_layer_visibility(bool p_visibility, const String &p_layer_name) {
    if (p_visibility) {
        int idx = hidden_layers.find(p_layer_name);
        if (idx != -1) {
            hidden_layers.remove_at(idx);
        }
    } else {
        if (hidden_layers.find(p_layer_name) == -1) {
            hidden_layers.append(p_layer_name);
        }
    }
}
