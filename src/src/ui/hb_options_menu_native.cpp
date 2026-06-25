#include "hb_options_menu_native.hpp"
#include "hb_game.hpp"
#include "utils/hb_input_native.hpp"
#include "graphics/hb_video_driver_psgl.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"
#include "compat/godot_cpp/classes/resource_loader.hpp"
#include <cmath>
#include <algorithm>

namespace godot {

HBOptionsMenuNative::HBOptionsMenuNative() {
    font = ResourceLoader::get_singleton()->load("res://fonts/orbitron/Orbitron-Regular.ttf");
    font_bold = ResourceLoader::get_singleton()->load("res://fonts/orbitron/Orbitron-Black.ttf");
    background_tex = ResourceLoader::get_singleton()->load("res://graphics/predarkenedbg.png");
    heart_tex = ResourceLoader::get_singleton()->load("res://graphics/heart.png");
    checkbox_on = ResourceLoader::get_singleton()->load("res://graphics/icons/checkbox-marked.png");
    checkbox_off = ResourceLoader::get_singleton()->load("res://graphics/icons/checkbox-blank-outline.png");
    
    settings = HBGameNative::get_singleton()->get_user_settings();

    // Initialize sections based on Godot implementation
    Section game_section;
    game_section.name = String("Juego");
    game_section.options.push_back(Option(String("Compensación de lag"), String("Retraso aplicado al timing de las notas."), String("range"), -300.0f, 300.0f, 1.0f, Array(), String("lag_compensation")));
    game_section.options.push_back(Option(String("Visualizador"), String("Habilita efectos de visualización de audio."), String("bool"), 0.0f, 1.0f, 1.0f, Array(), String("visualizer_enabled")));
    sections.push_back(game_section);

    Section video_section;
    video_section.name = String("Video");
    video_section.options.push_back(Option(String("Límite de FPS"), String("Limita los cuadros por segundo."), String("range"), 0.0f, 240.0f, 60.0f, Array(), String("fps_limit")));
    sections.push_back(video_section);

    Section visual_section;
    visual_section.name = String("Visual");
    visual_section.options.push_back(Option(String("Tamaño de nota"), String("Qué tan grandes aparecerán las notas."), String("range"), 0.1f, 3.0f, 0.1f, Array(), String("note_size")));
    Array icon_packs;
    icon_packs.push_back(String("playstation"));
    icon_packs.push_back(String("xbox"));
    icon_packs.push_back(String("nintendo"));
    visual_section.options.push_back(Option(String("Pack de iconos"), String("Cambia los iconos de los botones."), String("select"), 0.0f, 0.0f, 0.0f, icon_packs, String("icon_pack")));
    sections.push_back(visual_section);
}

void HBOptionsMenuNative::update() {
    time_passed += 0.016f;

    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_BACK)) {
        if (selected_option_index == -1) {
            HBGameNative::get_singleton()->change_to_menu(String("main_menu"));
            return;
        } else {
            selected_option_index = -1;
            return;
        }
    }

    if (selected_option_index == -1) {
        // Navigating sections
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_DOWN)) {
            selected_section_index = (selected_section_index + 1) % sections.size();
        }
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_UP)) {
            selected_section_index = (selected_section_index - 1 + sections.size()) % sections.size();
        }
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_ACCEPT)) {
            if (!sections[selected_section_index].options.empty()) {
                selected_option_index = 0;
            }
        }
    } else {
        // Navigating options
        Section& current_section = sections[selected_section_index];
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_DOWN)) {
            selected_option_index = (selected_option_index + 1) % current_section.options.size();
        }
        if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_UP)) {
            selected_option_index = (selected_option_index - 1 + current_section.options.size()) % current_section.options.size();
        }
        
        Option& opt = current_section.options[selected_option_index];
        if (opt.type == String("bool")) {
            if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_ACCEPT) || 
                HBInputNative::is_action_just_pressed(HBInputNative::ACTION_LEFT) || 
                HBInputNative::is_action_just_pressed(HBInputNative::ACTION_RIGHT)) {
                bool val = settings->get(opt.property);
                settings->set(opt.property, !val);
            }
        } else if (opt.type == String("range")) {
            if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_LEFT)) {
                float val = settings->get(opt.property);
                settings->set(opt.property, std::max(opt.min, val - opt.step));
            }
            if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_RIGHT)) {
                float val = settings->get(opt.property);
                settings->set(opt.property, std::min(opt.max, val + opt.step));
            }
        } else if (opt.type == String("select")) {
             if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_LEFT)) {
                String val = settings->get(opt.property);
                int idx = opt.choices.find(val);
                if (idx != -1) {
                    idx = (idx - 1 + opt.choices.size()) % opt.choices.size();
                    settings->set(opt.property, opt.choices[idx]);
                }
            }
            if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_RIGHT)) {
                String val = settings->get(opt.property);
                int idx = opt.choices.find(val);
                if (idx != -1) {
                    idx = (idx + 1) % opt.choices.size();
                    settings->set(opt.property, opt.choices[idx]);
                }
            }
        }
    }
}

void HBOptionsMenuNative::draw() {
    Vector2 window_size = HBVideoDriverPSGL::get_window_size();
    float scale_x = window_size.x / 1920.0f;
    float scale_y = window_size.y / 1080.0f;

    // Background
    if (background_tex.is_valid()) {
        HBVideoDriverPSGL::draw_texture(background_tex, Rect2(0, 0, window_size.x, window_size.y));
    }

    // Pulsing heart
    if (heart_tex.is_valid()) {
        float pulse = 1.0f + 0.05f * std::sin(time_passed * 2.0f);
        float heart_size = 800.0f * pulse;
        HBVideoDriverPSGL::draw_texture(heart_tex, Rect2((960 - heart_size / 2.0f) * scale_x, (540 - heart_size / 2.0f) * scale_y, heart_size * scale_x, heart_size * scale_y), Color(1, 1, 1, 0.25f));
    }

    _draw_profile_header(1.0f);
    _draw_options_panel(1.0f);
    _draw_hint_bar(1.0f);
}

void HBOptionsMenuNative::_draw_profile_header(float p_alpha) {
    Vector2 window_size = HBVideoDriverPSGL::get_window_size();
    float scale_x = window_size.x / 1920.0f;
    float scale_y = window_size.y / 1080.0f;

    HBVideoDriverPSGL::draw_text_with_font(font_bold, String("Usuario"), Vector2(1850 * scale_x, 50 * scale_y), (int)(24 * scale_y), Color(1, 1, 1, p_alpha), true);
}

void HBOptionsMenuNative::_draw_options_panel(float p_alpha) {
    Vector2 window_size = HBVideoDriverPSGL::get_window_size();
    float scale_x = window_size.x / 1920.0f;
    float scale_y = window_size.y / 1080.0f;

    float slant = 35.0f;
    Transform3D tilt = Transform3D::rotated(Vector3(0, 1, 0), 0.035f); // 2 degree tilt
    tilt = tilt.translated(Vector3(0, 0, -0.5f));

    // Draw sections on the left
    float section_x = 100 * scale_x;
    float section_y = 200 * scale_y;
    float section_w = 300 * scale_x;
    float section_h = 60 * scale_y;
    float section_sep = 10 * scale_y;

    for (size_t i = 0; i < sections.size(); i++) {
        Rect2 rect(section_x, section_y + i * (section_h + section_sep), section_w, section_h);
        Color col = (i == (size_t)selected_section_index) ? Color(0.929f, 0.216f, 0.8f, 0.8f * p_alpha) : Color(0.19f, 0.07f, 0.3f, 0.8f * p_alpha);
        HBVideoDriverPSGL::draw_rect_3d(rect, tilt, col, slant);
        HBVideoDriverPSGL::draw_text_with_font_3d(font_bold, sections[i].name, Vector2(section_x + 20 * scale_x, section_y + i * (section_h + section_sep) + 10 * scale_y), (int)(30 * scale_y), tilt, Color(1, 1, 1, p_alpha));
    }

    // Main panel
    Rect2 panel_rect(450 * scale_x, 150 * scale_y, 1300 * scale_x, 800 * scale_y);
    HBVideoDriverPSGL::draw_rect_3d(panel_rect, tilt, Color(0.19f, 0.07f, 0.3f, 0.85f * p_alpha), slant);

    // Draw options in the selected section
    Section& current_section = sections[selected_section_index];
    float opt_x = 500 * scale_x;
    float opt_y = 200 * scale_y;
    float opt_h = 80 * scale_y;

    for (size_t i = 0; i < current_section.options.size(); i++) {
        Option& opt = current_section.options[i];
        Color text_col = (i == (size_t)selected_option_index) ? Color(1, 1, 1, p_alpha) : Color(0.7f, 0.7f, 0.7f, p_alpha);
        HBVideoDriverPSGL::draw_text_with_font_3d(font, opt.name, Vector2(opt_x, opt_y + i * opt_h), (int)(28 * scale_y), tilt, text_col);
        
        // Value draw
        if (opt.type == String("bool")) {
             bool val = settings->get(opt.property);
             Ref<Image> tex = val ? checkbox_on : checkbox_off;
             if (tex.is_valid()) {
                HBVideoDriverPSGL::draw_texture_3d(tex, Rect2(opt_x + 1000 * scale_x, opt_y + i * opt_h, 40 * scale_x, 40 * scale_y), tilt, text_col);
             }
        } else if (opt.type == String("range")) {
             Variant val = settings->get(opt.property);
             String val_str;
             if (val.get_type() == Variant::FLOAT) {
                 val_str = String::num((double)val, 1);
             } else {
                 val_str = String::num_int64((int64_t)val);
             }
             HBVideoDriverPSGL::draw_text_with_font_3d(font, val_str, Vector2(opt_x + 1000 * scale_x, opt_y + i * opt_h), (int)(28 * scale_y), tilt, text_col);
        } else if (opt.type == String("select")) {
            String val = settings->get(opt.property);
            HBVideoDriverPSGL::draw_text_with_font_3d(font, val, Vector2(opt_x + 1000 * scale_x, opt_y + i * opt_h), (int)(28 * scale_y), tilt, text_col);
        }
    }

    // Footer info
    String footer_info = String("Release Candidate 5 | PS3/CellOS | yt-dlp 2023.03.04");
    HBVideoDriverPSGL::draw_text_with_font_3d(font, footer_info, Vector2(500 * scale_x, 900 * scale_y), (int)(18 * scale_y), tilt, Color(1, 1, 1, 0.5f * p_alpha));
}

void HBOptionsMenuNative::_draw_hint_bar(float p_alpha) {
    Vector2 window_size = HBVideoDriverPSGL::get_window_size();
    float scale_x = window_size.x / 1920.0f;
    float scale_y = window_size.y / 1080.0f;

    HBVideoDriverPSGL::draw_rect(Rect2(0, 1030 * scale_y, window_size.x, 50 * scale_y), Color(0, 0, 0, 0.5f * p_alpha));
    HBVideoDriverPSGL::draw_text_with_font(font, String("Seleccionar: X | Volver: O"), Vector2(100 * scale_x, 1045 * scale_y), (int)(20 * scale_y), Color(1, 1, 1, p_alpha));
}

}
