#ifndef HB_OPTIONS_MENU_NATIVE_HPP
#define HB_OPTIONS_MENU_NATIVE_HPP

#include "hb_song_loader.hpp"
#include "compat/godot_cpp/classes/node.hpp"
#include "compat/godot_cpp/classes/image.hpp"
#include "compat/godot_cpp/classes/font_variation.hpp"
#include "compat/godot_cpp/variant/vector2.hpp"
#include "compat/godot_cpp/variant/transform3d.hpp"
#include "chart/hb_user_settings.hpp"
#include <vector>

namespace godot {

class HBOptionsMenuNative : public Node {
public:
    struct Option {
        String name;
        String description;
        String type; // "bool", "range", "select"
        float min = 0.0f;
        float max = 1.0f;
        float step = 0.1f;
        Array choices;
        String property;

        Option(String p_name, String p_description, String p_type, float p_min, float p_max, float p_step, Array p_choices, String p_property) {
            name = p_name;
            description = p_description;
            type = p_type;
            min = p_min;
            max = p_max;
            step = p_step;
            choices = p_choices;
            property = p_property;
        }
    };

    struct Section {
        String name;
        std::vector<Option> options;
    };

private:
    std::vector<Section> sections;
    int selected_section_index = 0;
    int selected_option_index = -1; // -1 means selecting section

    Ref<FontVariation> font;
    Ref<FontVariation> font_bold;
    Ref<Image> background_tex;
    Ref<Image> heart_tex;
    Ref<Image> checkbox_on;
    Ref<Image> checkbox_off;
    Ref<Image> arrow_left;
    Ref<Image> arrow_right;

    float time_passed = 0.0f;
    Ref<HBUserSettingsNative> settings;

    void _draw_profile_header(float p_alpha);
    void _draw_options_panel(float p_alpha);
    void _draw_hint_bar(float p_alpha);

public:
    HBOptionsMenuNative();
    virtual ~HBOptionsMenuNative() {}
    void update();
    void draw();
};

}

#endif
