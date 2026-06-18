#include "hb_main_menu_native.hpp"
#include "utils/hb_input_native.hpp"
#include "graphics/hb_video_driver_psgl.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"
#include "compat/godot_cpp/classes/resource_loader.hpp"
#include <cmath>

namespace godot {

HBMainMenuNative::HBMainMenuNative() {
    menu_items.push_back({"Juego Libre", "song_list", ResourceLoader::get_singleton()->load("res://graphics/icons/music-box-outline.svg")});
    menu_items.push_back({"Workshop", "workshop_browser", ResourceLoader::get_singleton()->load("res://graphics/icons/steam.svg")});
    menu_items.push_back({"Como jugar", "tutorial", ResourceLoader::get_singleton()->load("res://graphics/icons/help-circle.svg")});
    menu_items.push_back({"Multijugador", "lobby_list", ResourceLoader::get_singleton()->load("res://graphics/icons/earth.svg")});
    menu_items.push_back({"Descargas", "download_queue", ResourceLoader::get_singleton()->load("res://graphics/icons/download.svg")});
    menu_items.push_back({"Opciones", "options_menu", ResourceLoader::get_singleton()->load("res://graphics/icons/settings.svg")});
    menu_items.push_back({"Herramientas", "tools_menu", ResourceLoader::get_singleton()->load("res://graphics/icons/tools.svg")});
    menu_items.push_back({"Salir", "exit", ResourceLoader::get_singleton()->load("res://graphics/icons/exit-run.svg")});

    logo = ResourceLoader::get_singleton()->load("res://graphics/Logo.png");
    heart = ResourceLoader::get_singleton()->load("res://graphics/heart.png");
    bokeh_tex = ResourceLoader::get_singleton()->load("res://graphics/icons/menu_heart_white.png");
    background_tex = ResourceLoader::get_singleton()->load("res://graphics/predarkenedbg.png");
    font = ResourceLoader::get_singleton()->load("res://fonts/Roboto-Regular.ttf");
    font_bold = ResourceLoader::get_singleton()->load("res://fonts/Roboto-Black.ttf");

    // Initialize bokeh circles
    for (int i = 0; i < 15; i++) {
        BokehCircle circle;
        circle.position = Vector2(UtilityFunctions::randf_range(0, 1920), UtilityFunctions::randf_range(0, 1080));
        circle.velocity = Vector2(UtilityFunctions::randf_range(-20, 20), UtilityFunctions::randf_range(-20, 20));
        circle.size = UtilityFunctions::randf_range(50, 150);
        circle.color = Color(0.31f, 0.19f, 0.68f, UtilityFunctions::randf_range(0.1f, 0.3f));
        bokeh_circles.push_back(circle);
    }
}

void HBMainMenuNative::update() {
    HBInputNative::update();
    time_passed += 0.016f; // Approx 60fps

    // Update bokeh circles
    for (size_t i = 0; i < bokeh_circles.size(); i++) {
        bokeh_circles[i].position += bokeh_circles[i].velocity * 0.016f;
        if (bokeh_circles[i].position.x < -200) bokeh_circles[i].position.x = 2120;
        if (bokeh_circles[i].position.x > 2120) bokeh_circles[i].position.x = -200;
        if (bokeh_circles[i].position.y < -200) bokeh_circles[i].position.y = 1280;
        if (bokeh_circles[i].position.y > 1280) bokeh_circles[i].position.y = -200;
    }

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
    // Draw background
    if (background_tex.is_valid()) {
        HBVideoDriverPSGL::draw_texture(background_tex, Rect2(0, 0, 1920, 1080));
    } else {
        HBVideoDriverPSGL::draw_rect(Rect2(0, 0, 1920, 1080), Color(0.13, 0.07, 0.26, 1.0));
    }

    // Draw bokeh
    for (size_t i = 0; i < bokeh_circles.size(); i++) {
        if (bokeh_tex.is_valid()) {
            HBVideoDriverPSGL::draw_texture(bokeh_tex, Rect2(bokeh_circles[i].position, Vector2(bokeh_circles[i].size, bokeh_circles[i].size)), bokeh_circles[i].color);
        } else {
            HBVideoDriverPSGL::draw_rect(Rect2(bokeh_circles[i].position, Vector2(bokeh_circles[i].size, bokeh_circles[i].size)), bokeh_circles[i].color);
        }
    }

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
        // Draw central heart
        if (heart.is_valid()) {
            float pulse = 1.0f + 0.05f * std::sin(time_passed * 2.0f);
            float heart_size = 600.0f * pulse;
            HBVideoDriverPSGL::draw_texture(heart, Rect2(960 - heart_size / 2.0f, 540 - heart_size / 2.0f, heart_size, heart_size), Color(1, 1, 1, 0.8f));
        }

        // Main Menu Layout matching PC
        if (logo.is_valid()) {
            HBVideoDriverPSGL::draw_texture(logo, Rect2(30, 240, 320, 160));
        }

        float button_width = 430;
        float button_height = 60;
        float separation = 10;
        float slant = -60.0f; // Approx from Godot's skew Vector2(-1, 0)
        int num_items = (int)menu_items.size();
        float start_y = 350.0f;

        for (int i = 0; i < num_items; i++) {
            float item_y = start_y + i * (button_height + separation);
            
            Color item_color;
            Color text_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
            if (i == selected_index) {
                // From NewButtonStyleHover.tres: Color(0.929412, 0.219608, 0.8, 0.501961)
                item_color = Color(0.93f, 0.22f, 0.8f, 0.5f);
                HBVideoDriverPSGL::draw_parallelogram(Rect2(50, item_y, button_width, button_height), slant, item_color);
            } else {
                // From NewButtonStyle.tres: Color(0.186, 0.072, 0.3, 0.501961)
                item_color = Color(0.19f, 0.07f, 0.3f, 0.5f);
                HBVideoDriverPSGL::draw_parallelogram(Rect2(50, item_y, button_width, button_height), slant, item_color);
                // Simulated left border from Godot: border_width_left = 100, border_color = Color(0.929, 0.22, 0.8, 0.5)
                HBVideoDriverPSGL::draw_parallelogram(Rect2(50, item_y, 40, button_height), slant, Color(0.93f, 0.22f, 0.8f, 0.5f));
            }
            
            // Icon
            if (menu_items[i].icon.is_valid()) {
                HBVideoDriverPSGL::draw_texture(menu_items[i].icon, Rect2(95, item_y + 10, 40, 40));
            }

            // Label
            HBVideoDriverPSGL::draw_text_with_font(font, menu_items[i].label, Vector2(150, item_y + 15), 24, text_color, true);
        }
    }
    
    // Footer
    HBVideoDriverPSGL::draw_text_with_font(font, "HeartbeatNET: Connected", Vector2(20, 1010), 18, Color(0.6, 0.6, 0.6, 0.8), true);
    HBVideoDriverPSGL::draw_text_with_font(font, "Project Heartbeat PS3 Marina's Legacy v1.0.0", Vector2(20, 1045), 18, Color(0.6, 0.6, 0.6, 0.8), true);
}

}
