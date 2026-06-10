#ifndef HB_PER_SONG_EDITOR_SETTINGS_HPP
#define HB_PER_SONG_EDITOR_SETTINGS_HPP

#include "hb_serializable.hpp"

namespace godot {

class HBPerSongEditorSettingsNative : public HBSerializableNative {
    GDCLASS(HBPerSongEditorSettingsNative, HBSerializableNative);

protected:
    static void _bind_methods();

private:
    Array hidden_layers;
    int32_t note_resolution = 16;
    bool timeline_snap = true;
    bool show_video = true;
    bool show_bg = true;
    int32_t selected_variant = -1;

public:
    HBPerSongEditorSettingsNative();
    ~HBPerSongEditorSettingsNative();

    void set_layer_visibility(bool p_visibility, const String &p_layer_name);

    virtual String get_serialized_type() const override { return "PerSongEditorSettings"; }
};

} // namespace godot

#endif // HB_PER_SONG_EDITOR_SETTINGS_HPP
