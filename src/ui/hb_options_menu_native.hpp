#ifndef HB_OPTIONS_MENU_NATIVE_HPP
#define HB_OPTIONS_MENU_NATIVE_HPP

#include "compat/godot_cpp/classes/node.hpp"
#include "compat/godot_cpp/classes/image.hpp"
#include "compat/godot_cpp/classes/font_variation.hpp"
#include "compat/godot_cpp/variant/variant.hpp"
#include <vector>

namespace godot {

class HBOptionsMenuNative : public Node {
public:
    enum OptionType {
        OPTION_BOOL,
        OPTION_RANGE,
        OPTION_SELECT
    };

    struct Option {
        String name;
        String description;
        OptionType type;
        String property_name; // To map to UserSettings
        float min_value = 0.0f;
        float max_value = 1.0f;
        float step = 0.1f;
        std::vector<String> select_options;
        std::vector<String> select_options_pretty;

        Option(String p_name, String p_desc, OptionType p_type, String p_prop)
            : name(p_name), description(p_desc), type(p_type), property_name(p_prop), min_value(0.0f), max_value(1.0f), step(0.1f) {}
        
        Option(String p_name, String p_desc, OptionType p_type, String p_prop, float p_min, float p_max, float p_step)
            : name(p_name), description(p_desc), type(p_type), property_name(p_prop), min_value(p_min), max_value(p_max), step(p_step) {}
    };

    struct Section {
        String name;
        std::vector<Option> options;
    };

private:
    std::vector<Section> sections;
    int selected_section = 0;
    int selected_option = -1; // -1 means section list is focused
    
    Ref<FontVariation> font;
    Ref<FontVariation> font_bold;
    Ref<Image> background_tex;
    
    float time_passed = 0.0f;

    void _setup_options();

public:
    HBOptionsMenuNative();
    void update();
    void draw();
};

}

#endif
