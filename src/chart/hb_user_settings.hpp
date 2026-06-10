#ifndef HB_USER_SETTINGS_HPP
#define HB_USER_SETTINGS_HPP

#include "hb_serializable.hpp"
#include "hb_folder.hpp"

namespace godot {

class HBUserSettingsNative : public HBSerializableNative {
    GDCLASS(HBUserSettingsNative, HBSerializableNative);

protected:
    static void _bind_methods();

private:
    bool visualizer_enabled = true;
    int32_t visualizer_resolution = 32;
    bool ui_blur_enabled = false;
    int32_t lag_compensation = 0;
    double note_size = 1.0;
    String icon_pack = "playstation";
    String resource_pack = "playstation";
    String ui_skin;

    Dictionary input_map;
    int32_t fps_limit = 180;
    String display_mode = "borderless";
    int32_t display = -1;

    Ref<HBFolderNative> root_folder;

public:
    HBUserSettingsNative();
    ~HBUserSettingsNative();

    virtual String get_serialized_type() const override { return "UserSettings"; }
};

} // namespace godot

#endif // HB_USER_SETTINGS_HPP
