#include "hb_main_menu_native.hpp"
#include "utils/hb_input_native.hpp"
#include "graphics/hb_video_driver_psgl.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"
#include "compat/godot_cpp/classes/resource_loader.hpp"
#include <cmath>

namespace godot {

HBMainMenuNative::HBMainMenuNative() {
    menu_items.push_back({"Free Play", "song_list", ResourceLoader::get_singleton()->load("res://graphics/icons/music-box-outline.svg")});
    menu_items.push_back({"Workshop", "workshop_browser", ResourceLoader::get_singleton()->load("res://graphics/icons/steam.svg")});
    menu_items.push_back({"How to play", "tutorial", ResourceLoader::get_singleton()->load("res://graphics/icons/help-circle.svg")});
    menu_items.push_back({"Multiplayer", "lobby_list", ResourceLoader::get_singleton()->load("res://graphics/icons/earth.svg")});
    menu_items.push_back({"Downloads", "download_queue", ResourceLoader::get_singleton()->load("res://graphics/icons/download.svg")});
    menu_items.push_back({"Options", "options_menu", ResourceLoader::get_singleton()->load("res://graphics/icons/settings.svg")});
    menu_items.push_back({"Tools", "tools_menu", ResourceLoader::get_singleton()->load("res://graphics/icons/tools.svg")});
    menu_items.push_back({"Exit", "exit", ResourceLoader::get_singleton()->load("res://graphics/icons/exit-run.svg")});

    logo = ResourceLoader::get_singleton()->load("res://graphics/Logo.png");
    font = ResourceLoader::get_singleton()->load("res://fonts/Roboto-Regular.ttf");
    font_bold = ResourceLoader::get_singleton()->load("res://fonts/Roboto-Black.ttf");
}

void HBMainMenuNative::update() {
    HBInputNative::update();
    time_passed += 0.016f; // Approx 60fps

    if (state == PRESS_START) {
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_ACCEPT) || 
            HBInputNative::is_action_just_pressed(HBInputNative::ACTION_UP) ||
            HBInputNative::is_action_just_pressed(HBInputNative::ACTION_DOWN)) {
            state = MAIN_MENU;
        }
    } else if (state == MAIN_MENU) {
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_DOWN)) {
            selected_index = (selected_index + 1) % menu_items.size();
        }
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_UP)) {
            selected_index = (selected_index - 1 + menu_items.size()) % menu_items.size();
        }
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_ACCEPT)) {
            UtilityFunctions::print("Menu: ACCEPT: " + menu_items[selected_index].label);
        }
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_BACK)) {
            state = PRESS_START;
        }
    }
}

void HBMainMenuNative::draw() {
    // Draw background (matching PC's dark theme)
    HBVideoDriverPSGL::draw_rect(Rect2(0, 0, 1920, 1080), Color(0.01f, 0.01f, 0.04f, 1.0f));

    if (state == PRESS_START) {
        // Draw logo centered
        if (logo.is_valid()) {
            HBVideoDriverPSGL::draw_texture(logo, Rect2(560, 200, 800, 400));
        } else {
            HBVideoDriverPSGL::draw_text_with_font(font_bold, "Project Heartbeat", Vector2(650, 350), 48, Color(1, 1, 1, 1), true);
        }

        // Pulse "Press Start"
        float alpha = 0.6f + 0.4f * std::sin(time_passed * 4.0f);
        HBVideoDriverPSGL::draw_text_with_font(font, "PRESS START", Vector2(810, 800), 36, Color(1, 1, 1, alpha), true);
    } else {
        // Main Menu Layout matching PC's MainMenuLeft.tscn
        if (logo.is_valid()) {
            HBVideoDriverPSGL::draw_texture(logo, Rect2(2, 62, 800, 400));
        } else {
            HBVideoDriverPSGL::draw_text_with_font(font_bold, "Project Heartbeat", Vector2(100, 200), 32, Color(1, 1, 1, 1), true);
        }

        float button_width = 430;
        float button_height = 60;
        float separation = 20; // Exact match to MainMenuLeft.tscn
        int num_items = (int)menu_items.size();
        float total_height = num_items * button_height + (num_items - 1) * separation;
        float start_y = (1080 - total_height) / 2.0f + 120.0f; // Shifted slightly for PS3 aspect

        for (int i = 0; i < num_items; i++) {
            float item_y = start_y + i * (button_height + separation);
            
            Color item_color = Color(0.08f, 0.08f, 0.1f, 0.9f);
            Color text_color = Color(0.85f, 0.85f, 0.85f, 1.0f);
            if (i == selected_index) {
                // Exact PC highlight blue
                item_color = Color(0.17f, 0.47f, 0.88f, 1.0f);
                text_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
                // Selection border glow
                HBVideoDriverPSGL::draw_rect(Rect2(48, item_y - 4, button_width + 10, button_height + 8), Color(1.0f, 1.0f, 1.0f, 0.25f));
            }

            HBVideoDriverPSGL::draw_rect(Rect2(53, item_y, button_width, button_height), item_color);
            
            // Icon
            if (menu_items[i].icon.is_valid()) {
                HBVideoDriverPSGL::draw_texture(menu_items[i].icon, Rect2(65, item_y + 10, 40, 40));
            } else {
                HBVideoDriverPSGL::draw_rect(Rect2(65, item_y + 10, 40, 40), Color(1.0f, 1.0f, 1.0f, 0.3f));
            }

            // Label with shadow
            HBVideoDriverPSGL::draw_text_with_font(font, menu_items[i].label, Vector2(120, item_y + 15), 24, text_color, true);
        }
    }
    
    // Footer
    HBVideoDriverPSGL::draw_text_with_font(font, "HeartbeatNET: Connected", Vector2(20, 1010), 18, Color(0.6, 0.6, 0.6, 0.8), true);
    HBVideoDriverPSGL::draw_text_with_font(font, "Project Heartbeat PS3 Marina's Legacy v1.0.0", Vector2(20, 1045), 18, Color(0.6, 0.6, 0.6, 0.8), true);
}

}
