#ifndef HB_MAIN_MENU_NATIVE_HPP
#define HB_MAIN_MENU_NATIVE_HPP

#include "hb_song_loader.hpp"
#include "compat/godot_cpp/classes/node.hpp"

namespace godot {

class HBMainMenuNative : public Node {
    int selected_song_index = 0;
    Array song_titles;

public:
    HBMainMenuNative();
    void update();
    void draw();
};

}

#endif
