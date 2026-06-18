#ifndef HB_MAIN_MENU_NATIVE_HPP
#define HB_MAIN_MENU_NATIVE_HPP

#include "hb_song_loader.hpp"
#include "compat/godot_cpp/classes/node.hpp"
#include "compat/godot_cpp/classes/image.hpp"
#include "compat/godot_cpp/classes/font_variation.hpp"
#include <vector>

namespace godot {

class HBMainMenuNative : public Node {
    struct MenuItem {
        String label;
        String next_menu;
        Ref<Image> icon;
    };
    
    enum State {
        PRESS_START,
        MAIN_MENU
    };

    State state = PRESS_START;
    int selected_index = 0;
    std::vector<MenuItem> menu_items;
    Ref<Image> logo;
    Ref<FontVariation> font;
    Ref<FontVariation> font_bold;
    float time_passed = 0.0f;

public:
    HBMainMenuNative();
    void update();
    void draw();
};

}

#endif
