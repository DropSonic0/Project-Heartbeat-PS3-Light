#include "hb_options_menu_native.hpp"
#include "hb_game.hpp"
#include "utils/hb_input_native.hpp"
#include "graphics/hb_video_driver_psgl.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"
#include "compat/godot_cpp/classes/resource_loader.hpp"
#include <cmath>

namespace godot {

HBOptionsMenuNative::HBOptionsMenuNative() {
    font = ResourceLoader::get_singleton()->load("res://fonts/orbitron/Orbitron-Regular.ttf");
    font_bold = ResourceLoader::get_singleton()->load("res://fonts/orbitron/Orbitron-Black.ttf");
    background_tex = ResourceLoader::get_singleton()->load("res://graphics/predarkenedbg.png");

    _setup_options();
}

void HBOptionsMenuNative::_setup_options() {
    // Audio Section
    Section audio;
    audio.name = "Audio";
    audio.options.push_back({"Volumen Maestro", "Volumen global del juego", OPTION_RANGE, "master_volume", 0.0f, 1.5f, 0.05f});
    audio.options.push_back({"Volumen Música", "Volumen de la música", OPTION_RANGE, "music_volume", 0.0f, 1.5f, 0.05f});
    audio.options.push_back({"Volumen Efectos", "Volumen de los efectos de sonido", OPTION_RANGE, "sfx_volume", 0.0f, 1.5f, 0.05f});
    sections.push_back(audio);

    // Visual Section
    Section visual;
    visual.name = "Visual";
    visual.options.push_back({"Tamaño de Notas", "Escala de las notas en el juego", OPTION_RANGE, "note_size", 0.5f, 2.0f, 0.1f});
    visual.options.push_back({"Oscurecer Fondo", "Oscurece el fondo durante el juego", OPTION_RANGE, "background_dim", 0.0f, 1.0f, 0.05f});
    visual.options.push_back({"Visualizador", "Habilita el visualizador de audio", OPTION_BOOL, "visualizer_enabled"});
    sections.push_back(visual);

    // Gameplay Section
    Section gameplay;
    gameplay.name = "Juego";
    gameplay.options.push_back({"Compensación de Lag", "Ajuste de sincronización en ms", OPTION_RANGE, "lag_compensation", -300.0f, 300.0f, 1.0f});
    gameplay.options.push_back({"Mostrar Latencia", "Muestra qué tan temprano o tarde golpeas", OPTION_BOOL, "show_latency"});
    sections.push_back(gameplay);
    
    // Input Section
    Section input;
    input.name = "Controles";
    input.options.push_back({"Vibración", "Habilita la vibración del mando", OPTION_BOOL, "enable_vibration"});
    input.options.push_back({"Zona Muerta", "Zona muerta de los sticks analógicos", OPTION_RANGE, "analog_deadzone", 0.05f, 0.5f, 0.05f});
    sections.push_back(input);

    // Credits Section
    Section credits;
    credits.name = "Créditos";
    sections.push_back(credits);
}

void HBOptionsMenuNative::update() {
    HBInputNative::update();
    time_passed += 0.016f;

    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_BACK)) {
        if (selected_option != -1) {
            selected_option = -1;
        } else {
            HBGameNative::get_singleton()->change_to_menu("main_menu", "skip_splash");
            return;
        }
    }

    if (selected_option == -1) {
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_DOWN)) {
            selected_section = (selected_section + 1) % sections.size();
        }
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_UP)) {
            selected_section = (selected_section - 1 + sections.size()) % sections.size();
        }
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_ACCEPT)) {
            if (!sections[selected_section].options.empty()) {
                selected_option = 0;
            }
        }
    } else {
        std::vector<Option>& options = sections[selected_section].options;
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_DOWN)) {
            selected_option = (selected_option + 1) % options.size();
        }
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_UP)) {
            selected_option = (selected_option - 1 + options.size()) % options.size();
        }
        
        Option& opt = options[selected_option];
        if (opt.type == OPTION_BOOL) {
            if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_ACCEPT) || 
                HBInputNative::is_action_just_pressed(HBInputNative::ACTION_LEFT) || 
                HBInputNative::is_action_just_pressed(HBInputNative::ACTION_RIGHT)) {
                // Toggle bool
                // For now we don't have a global settings object accessible, so we just print
                UtilityFunctions::print("Toggled " + opt.name);
            }
        } else if (opt.type == OPTION_RANGE) {
            if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_LEFT)) {
                UtilityFunctions::print("Decreased " + opt.name);
            }
            if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_RIGHT)) {
                UtilityFunctions::print("Increased " + opt.name);
            }
        }
    }
}

void HBOptionsMenuNative::draw() {
    Vector2 window_size = HBVideoDriverPSGL::get_window_size();
    float scale_x = window_size.x / 1920.0f;
    float scale_y = window_size.y / 1080.0f;

    // Draw background
    if (background_tex.is_valid()) {
        HBVideoDriverPSGL::draw_texture(background_tex, Rect2(0, 0, window_size.x, window_size.y));
    } else {
        HBVideoDriverPSGL::draw_rect(Rect2(0, 0, window_size.x, window_size.y), Color(0.129, 0.071, 0.259, 1.0));
    }

    HBVideoDriverPSGL::draw_text_with_font(font_bold, "Opciones", Vector2(100 * scale_x, 80 * scale_y), (int)(50 * scale_y), Color(1, 1, 1, 1));

    float section_width = 400.0f * scale_x;
    float section_height = 60.0f * scale_y;
    float start_y = 180.0f * scale_y;
    float separation = 10.0f * scale_y;

    // Draw sections list
    for (size_t i = 0; i < sections.size(); i++) {
        float item_y = start_y + i * (section_height + separation);
        Color item_color = (i == (size_t)selected_section) ? Color(0.73f, 0.22f, 0.52f, 0.8f) : Color(0.186f, 0.072f, 0.3f, 0.8f);
        if (selected_option != -1 && i == (size_t)selected_section) {
            item_color.a = 0.4f; // Dim when focus is on options
        }
        
        HBVideoDriverPSGL::draw_parallelogram(Rect2(60 * scale_x, item_y, section_width, section_height), -35.0f * scale_x, item_color);
        HBVideoDriverPSGL::draw_text_with_font(font, sections[i].name, Vector2(100 * scale_x, item_y + 12 * scale_y), (int)(34 * scale_y), Color(1, 1, 1, 1));
    }

    // Draw options for selected section
    if (selected_section >= 0 && selected_section < (int)sections.size()) {
        const std::vector<Option>& options = sections[selected_section].options;
        float opt_start_x = 520.0f * scale_x;
        float opt_width = 1300.0f * scale_x;
        
        if (sections[selected_section].name == "Créditos") {
            HBVideoDriverPSGL::draw_text_with_font(font, "Project Heartbeat PS3 Port", Vector2(opt_start_x, start_y), (int)(30 * scale_y), Color(1, 1, 1, 1));
            HBVideoDriverPSGL::draw_text_with_font(font, "Original by: EIREXE", Vector2(opt_start_x, start_y + 40 * scale_y), (int)(24 * scale_y), Color(1, 1, 1, 0.8f));
            HBVideoDriverPSGL::draw_text_with_font(font, "PS3 Lead: Jules", Vector2(opt_start_x, start_y + 80 * scale_y), (int)(24 * scale_y), Color(1, 1, 1, 0.8f));
            HBVideoDriverPSGL::draw_text_with_font(font, "Special thanks to: The Project Heartbeat community", Vector2(opt_start_x, start_y + 120 * scale_y), (int)(24 * scale_y), Color(1, 1, 1, 0.8f));
        } else {
            for (size_t i = 0; i < options.size(); i++) {
                float item_y = start_y + i * (section_height + separation);
                Color item_color = (selected_option == (int)i) ? Color(0.73f, 0.22f, 0.52f, 0.8f) : Color(0.186f, 0.072f, 0.3f, 0.6f);
                
                HBVideoDriverPSGL::draw_parallelogram(Rect2(opt_start_x, item_y, opt_width, section_height), -35.0f * scale_x, item_color);
                HBVideoDriverPSGL::draw_text_with_font(font, options[i].name, Vector2((opt_start_x + 40) * scale_x, item_y + 12 * scale_y), (int)(34 * scale_y), Color(1, 1, 1, 1));
                
                // Draw value (stubbed for now)
                String val_text = "[ STUB ]";
                if (options[i].type == OPTION_BOOL) val_text = "SI";
                HBVideoDriverPSGL::draw_text_with_font(font, val_text, Vector2((opt_start_x + opt_width - 150) * scale_x, item_y + 12 * scale_y), (int)(34 * scale_y), Color(1, 1, 1, 1), false, false);
            }
            
            // Draw description of selected option
            if (selected_option != -1) {
                float desc_y = window_size.y - 100 * scale_y;
                HBVideoDriverPSGL::draw_text_with_font(font, options[selected_option].description, Vector2(520 * scale_x, desc_y), (int)(24 * scale_y), Color(1, 1, 1, 0.7f));
            }
        }
    }
}

}
