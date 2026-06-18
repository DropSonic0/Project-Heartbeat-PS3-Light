#include "hb_main_menu_native.hpp"
#include "utils/hb_input_native.hpp"
#include "graphics/hb_video_driver_psgl.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"
#include "compat/godot_cpp/classes/resource_loader.hpp"
#include <cmath>

namespace godot {

HBMainMenuNative::HBMainMenuNative() {
    UtilityFunctions::randomize();
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
    font = ResourceLoader::get_singleton()->load("res://fonts/orbitron/Orbitron-Regular.ttf");
    font_bold = ResourceLoader::get_singleton()->load("res://fonts/orbitron/Orbitron-Black.ttf");

    std::vector<String> quotes;
    quotes.push_back(":v");
    quotes.push_back("¡Hola, PS3!");
    quotes.push_back("Marina's Legacy");
    quotes.push_back("Project Heartbeat");
    quotes.push_back("Keep on drumming!");
    quotes.push_back("Powered by PSGL");
    quotes.push_back("Made with <3");
    quotes.push_back("Cha-cha now y'all");
    quotes.push_back("Imagine the nerves");
    quotes.push_back("Original handcrafted code!");
    quotes.push_back("Now featuring 100% more leaderboards");
    quotes.push_back("Powered by Godot");
    quotes.push_back("Made with love, from Spain");
    quotes.push_back("↑ ↑ ↓ ↓ ← → ← → X O");
    quotes.push_back("Tontoelquelolea");
    quotes.push_back("Como que 33?");
    quotes.push_back("An offer you can't refuse!");
    quotes.push_back("Pick up that can");
    quotes.push_back("Dangerous choking hazard!");
    quotes.push_back("No plastic instruments required!");
    quotes.push_back("Excellent-o");
    quotes.push_back("You're winner!");
    quotes.push_back("Bwoah");
    quotes.push_back("Just leave me alone, I know what to do");
    quotes.push_back("Steam Deck compatible!");
    quotes.push_back("Better on Linux (tm)");
    quotes.push_back("Illo Illo Illo");
    quotes.push_back("DIVALLER compatible");
    quotes.push_back("Uoooooooh");
    quotes.push_back("Feliz jueves... no es jue... oh es jueves, es jueves...");
    quotes.push_back("Project Project Disappointment");
    
    current_quote = quotes[UtilityFunctions::randi() % quotes.size()];

    // Initialize bokeh circles
    for (int i = 0; i < 40; i++) {
        BokehCircle circle;
        circle.position = Vector2(UtilityFunctions::randf_range(0, 1920), UtilityFunctions::randf_range(0, 1080));
        circle.velocity = Vector2(UtilityFunctions::randf_range(-10, 10), UtilityFunctions::randf_range(-10, 10));
        circle.size = UtilityFunctions::randf_range(20, 80);
        circle.color = Color(0.4f, 0.3f, 0.8f, UtilityFunctions::randf_range(0.01f, 0.05f));
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
    Vector2 window_size = HBVideoDriverPSGL::get_window_size();
    float scale_x = window_size.x / 1920.0f;
    float scale_y = window_size.y / 1080.0f;

    // Draw background
    if (background_tex.is_valid()) {
        HBVideoDriverPSGL::draw_texture(background_tex, Rect2(0, 0, window_size.x, window_size.y));
    } else {
        HBVideoDriverPSGL::draw_rect(Rect2(0, 0, window_size.x, window_size.y), Color(0.129, 0.071, 0.259, 1.0));
    }

    // Draw bokeh
    for (size_t i = 0; i < bokeh_circles.size(); i++) {
        Vector2 pos = Vector2(bokeh_circles[i].position.x * scale_x, bokeh_circles[i].position.y * scale_y);
        Vector2 size = Vector2(bokeh_circles[i].size * scale_x, bokeh_circles[i].size * scale_y);
        if (bokeh_tex.is_valid()) {
            HBVideoDriverPSGL::draw_texture(bokeh_tex, Rect2(pos, size), bokeh_circles[i].color);
        } else {
            HBVideoDriverPSGL::draw_rect(Rect2(pos, size), bokeh_circles[i].color);
        }
    }

    if (state == PRESS_START) {
        // Draw logo centered with shadow
        if (logo.is_valid()) {
            float logo_orig_w = logo->get_width();
            float logo_orig_h = logo->get_height();
            float logo_w = 1050.0f;
            float logo_h = logo_w * (logo_orig_h / logo_orig_w);
            float logo_x = 960.0f - logo_w / 2.0f;
            float logo_y = 420.0f - logo_h / 2.0f;

            // Simulated Shadow
            HBVideoDriverPSGL::draw_texture(logo, Rect2((logo_x + 10) * scale_x, (logo_y + 10) * scale_y, logo_w * scale_x, logo_h * scale_y), Color(0.0f, 0.0f, 0.0f, 0.6f));
            HBVideoDriverPSGL::draw_texture(logo, Rect2(logo_x * scale_x, logo_y * scale_y, logo_w * scale_x, logo_h * scale_y));
            
            // Draw Quote
            HBVideoDriverPSGL::draw_text_with_font(font_bold, current_quote, Vector2(960 * scale_x, (logo_y + logo_h + 70) * scale_y), 39 * scale_y, Color(1, 1, 1, 0.6f), true, true);
        } else {
            HBVideoDriverPSGL::draw_text_with_font(font_bold, "Project Heartbeat", Vector2(960 * scale_x, 400 * scale_y), 64 * scale_y, Color(1, 1, 1, 1), true, true);
        }

        // Pulse "Presiona cualquier botón"
        float alpha = 0.6f + 0.4f * std::sin(time_passed * 4.0f);
        HBVideoDriverPSGL::draw_text_with_font(font_bold, "Presiona cualquier botón", Vector2(960 * scale_x, 765 * scale_y), 39 * scale_y, Color(1, 1, 1, alpha), true, true);
    } else {
        // Draw central heart
        if (heart.is_valid()) {
            float pulse = 1.0f + 0.05f * std::sin(time_passed * 2.0f);
            float heart_size = 600.0f * pulse;
            HBVideoDriverPSGL::draw_texture(heart, Rect2((960 - heart_size / 2.0f) * scale_x, (540 - heart_size / 2.0f) * scale_y, heart_size * scale_x, heart_size * scale_y), Color(1, 1, 1, 0.8f));
        }

        // Main Menu Layout matching PC
        if (logo.is_valid()) {
            float logo_orig_w = logo->get_width();
            float logo_orig_h = logo->get_height();
            float logo_w = 450.0f;
            float logo_h = logo_w * (logo_orig_h / logo_orig_w);
            // Shadow for menu logo too
            HBVideoDriverPSGL::draw_texture(logo, Rect2(65 * scale_x, 125 * scale_y, logo_w * scale_x, logo_h * scale_y), Color(0.0f, 0.0f, 0.0f, 0.6f));
            HBVideoDriverPSGL::draw_texture(logo, Rect2(60 * scale_x, 120 * scale_y, logo_w * scale_x, logo_h * scale_y));
        }

        float button_width = 460 * scale_x;
        float button_height = 60 * scale_y;
        float separation = 10 * scale_y;
        float slant = -60.0f * scale_x; // Approx from Godot's skew Vector2(-1, 0)
        int num_items = (int)menu_items.size();
        float start_y = 320.0f * scale_y;

        for (int i = 0; i < num_items; i++) {
            float item_y = start_y + i * (button_height + separation);
            
            Color item_color;
            Color text_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
            if (i == selected_index) {
                // From NewButtonStyleHover.tres: Color(0.929412, 0.219608, 0.8, 0.501961)
                item_color = Color(0.93f, 0.22f, 0.8f, 0.5f);
                HBVideoDriverPSGL::draw_parallelogram(Rect2(50 * scale_x, item_y, button_width, button_height), slant, item_color);
            } else {
                // From NewButtonStyle.tres: Color(0.186, 0.072, 0.3, 0.501961)
                item_color = Color(0.19f, 0.07f, 0.3f, 0.5f);
                HBVideoDriverPSGL::draw_parallelogram(Rect2(50 * scale_x, item_y, button_width, button_height), slant, item_color);
                // Simulated left border from Godot: border_width_left = 100, border_color = Color(0.929, 0.22, 0.8, 0.5)
                HBVideoDriverPSGL::draw_parallelogram(Rect2(50 * scale_x, item_y, 40 * scale_x, button_height), slant, Color(0.93f, 0.22f, 0.8f, 0.5f));
            }
            
            // Icon
            if (menu_items[i].icon.is_valid()) {
                HBVideoDriverPSGL::draw_texture(menu_items[i].icon, Rect2(95 * scale_x, item_y + 10 * scale_y, 40 * scale_x, 40 * scale_y));
            }

            // Label
        HBVideoDriverPSGL::draw_text_with_font(font, menu_items[i].label, Vector2(155 * scale_x, item_y + 6 * scale_y), 34 * scale_y, text_color, true);
        }

        // Footer - Only in MAIN_MENU
        HBVideoDriverPSGL::draw_text_with_font(font, "HeartbeatNET: Connected", Vector2(20 * scale_x, 1030 * scale_y), 14 * scale_y, Color(1, 1, 1, 0.4f), true);
        HBVideoDriverPSGL::draw_text_with_font(font, "Project Heartbeat PS3 Marina's Legacy v1.0.0", Vector2(20 * scale_x, 1055 * scale_y), 14 * scale_y, Color(1, 1, 1, 0.4f), true);
    }
}

}
