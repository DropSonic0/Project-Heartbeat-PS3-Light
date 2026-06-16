#include "hb_main_menu_native.hpp"
#include "utils/hb_input_native.hpp"
#include "graphics/hb_video_driver_psgl.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"

namespace godot {

HBMainMenuNative::HBMainMenuNative() {
    HBSongLoaderNative* loader = HBSongLoaderNative::get_singleton();
    if (loader) {
        Dictionary songs = loader->get_songs();
        song_titles = songs.keys();
    }
    if (song_titles.size() == 0) {
        song_titles.append("No songs found in PCK/USRDIR");
    }
}

void HBMainMenuNative::update() {
    HBInputNative::update();

    if (song_titles.size() > 0) {
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_DOWN)) {
            selected_song_index = (selected_song_index + 1) % song_titles.size();
            UtilityFunctions::print("Menu: Selected song: " + (String)song_titles[selected_song_index]);
        }
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_UP)) {
            selected_song_index = (selected_song_index - 1 + song_titles.size()) % song_titles.size();
            UtilityFunctions::print("Menu: Selected song: " + (String)song_titles[selected_song_index]);
        }
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_ACCEPT)) {
            UtilityFunctions::print("Menu: ACCEPT song: " + (String)song_titles[selected_song_index]);
        }
    }
}

void HBMainMenuNative::draw() {
    // Draw background (dark blueish gray)
    HBVideoDriverPSGL::draw_rect(Rect2(0, 0, 1920, 1080), Color(0.05f, 0.05f, 0.15f, 1.0f));

    // Draw a header bar
    HBVideoDriverPSGL::draw_rect(Rect2(0, 0, 1920, 80), Color(0.1f, 0.1f, 0.1f, 1.0f));

    // Draw song list items
    for (int i = 0; i < song_titles.size(); i++) {
        float item_y = 100 + i * 60;
        
        Color item_color = Color(0.2f, 0.2f, 0.2f, 0.8f);
        if (i == selected_song_index) {
            // Selected item: Light blue
            item_color = Color(0.2f, 0.5f, 0.9f, 1.0f);
            // Draw a selection highlight border
            HBVideoDriverPSGL::draw_rect(Rect2(95, item_y - 5, 1730, 60), Color(1.0f, 1.0f, 1.0f, 0.5f));
        }

        HBVideoDriverPSGL::draw_rect(Rect2(100, item_y, 1720, 50), item_color);
        
        // Draw a small "thumbnail" placeholder
        HBVideoDriverPSGL::draw_rect(Rect2(110, item_y + 5, 40, 40), Color(0.8f, 0.8f, 0.8f, 1.0f));
    }
    
    // In a real port we would draw text here using PSGL and a font loader.
    // For now, we rely on console output for names.
}

}
