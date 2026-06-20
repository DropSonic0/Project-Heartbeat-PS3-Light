#ifndef HB_MAIN_MENU_NATIVE_HPP
#define HB_MAIN_MENU_NATIVE_HPP

#include "hb_song_loader.hpp"
#include "compat/godot_cpp/classes/node.hpp"
#include "compat/godot_cpp/classes/image.hpp"
#include "compat/godot_cpp/variant/vector2.hpp"
#include "compat/godot_cpp/classes/font_variation.hpp"
#include <vector>

namespace godot {

class HBMainMenuNative : public Node {
public:
    struct MenuItem {
        String label;
        String next_menu;
        Ref<Image> icon;
    };
    
    enum State {
        PRESS_START,
        MAIN_MENU
    };

    struct BokehCircle {
        Vector2 position;
        Vector2 velocity;
        float size;
        Color color;
    };

private:
    State state = PRESS_START;
    float transition_progress = 0.0f;
    bool is_transitioning = false;
    int selected_index = 0;
    std::vector<MenuItem> menu_items;
    std::vector<BokehCircle> bokeh_circles;
    Ref<Image> logo;
    Ref<Image> heart;
    Ref<Image> visualizer_tex;
    Ref<Image> visualizer_lut;
    Ref<Image> bokeh_tex;
    Ref<Image> background_tex;
    Ref<FontVariation> font;
    Ref<FontVariation> font_bold;
    String current_quote;
    float time_passed = 0.0f;
    float spectrum_values[200];

public:
    HBMainMenuNative(State p_initial_state = PRESS_START);
    void update();
    void draw();
};

}

#endif
