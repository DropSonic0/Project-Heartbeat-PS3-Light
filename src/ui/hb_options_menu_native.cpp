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
    selected_section_index = 0;
    selected_option_index = -1;
    time_passed = 0.0f;

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
    
    Option lag_opt;
    lag_opt.name = String("Compensación de lag");
    lag_opt.description = String("Retraso aplicado al timing de las notas.");
    lag_opt.type = String("range");
    lag_opt.min = -300.0f;
    lag_opt.max = 300.0f;
    lag_opt.step = 1.0f;
    lag_opt.choices = Array();
    lag_opt.property = String("lag_compensation");
    game_section.options.push_back(lag_opt);

    Option vis_opt;
    vis_opt.name = String("Visualizador");
    vis_opt.description = String("Habilita efectos de visualización de audio.");
    vis_opt.type = String("bool");
    vis_opt.min = 0.0f;
    vis_opt.max = 1.0f;
    vis_opt.step = 1.0f;
    vis_opt.choices = Array();
    vis_opt.property = String("visualizer_enabled");
    game_section.options.push_back(vis_opt);
    
    sections.push_back(game_section);

    Section video_section;
    video_section.name = String("Video");
    
    Option fps_opt;
    fps_opt.name = String("Límite de FPS");
    fps_opt.description = String("Limita los cuadros por segundo.");
    fps_opt.type = String("range");
    fps_opt.min = 0.0f;
    fps_opt.max = 240.0f;
    fps_opt.step = 60.0f;
    fps_opt.choices = Array();
    fps_opt.property = String("fps_limit");
    video_section.options.push_back(fps_opt);
    
    sections.push_back(video_section);

    Section visual_section;
    visual_section.name = String("Visual");
    
    Option size_opt;
    size_opt.name = String("Tamaño de nota");
    size_opt.description = String("Qué tan grandes aparecerán las notas.");
    size_opt.type = String("range");
    size_opt.min = 0.1f;
    size_opt.max = 3.0f;
    size_opt.step = 0.1f;
    size_opt.choices = Array();
    size_opt.property = String("note_size");
    visual_section.options.push_back(size_opt);

    Array icon_packs;
    icon_packs.push_back(String("playstation"));
    icon_packs.push_back(String("xbox"));
    icon_packs.push_back(String("nintendo"));
    
    Option pack_opt;
    pack_opt.name = String("Pack de notas");
    pack_opt.description = String("Cambia los iconos de los botones.");
    pack_opt.type = String("select");
    pack_opt.min = 0.0f;
    pack_opt.max = 0.0f;
    pack_opt.step = 0.0f;
    pack_opt.choices = icon_packs;
    pack_opt.property = String("icon_pack");
    visual_section.options.push_back(pack_opt);
    
    sections.push_back(visual_section);
}

void HBOptionsMenuNative::update() {
    HBInputNative::update();
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
                bool val = settings->get(std::string(opt.property.c_str()));
                settings->set(std::string(opt.property.c_str()), (bool)!val);
            }
        } else if (opt.type == String("range")) {
            if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_LEFT)) {
                float val = settings->get(std::string(opt.property.c_str()));
                settings->set(std::string(opt.property.c_str()), (float)std::max(opt.min, val - opt.step));
            }
            if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_RIGHT)) {
                float val = settings->get(std::string(opt.property.c_str()));
                settings->set(std::string(opt.property.c_str()), (float)std::min(opt.max, val + opt.step));
            }
        } else if (opt.type == String("select")) {
            if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_LEFT)) {
                String val = settings->get(std::string(opt.property.c_str()));
                int idx = opt.choices.find(val);
                if (idx != -1) {
                    idx = (idx - 1 + opt.choices.size()) % opt.choices.size();
                } else {
                    idx = (int)opt.choices.size() - 1;
                }
                if (idx >= 0 && idx < (int)opt.choices.size()) {
                    settings->set(std::string(opt.property.c_str()), opt.choices[idx]);
                }
            }
            if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_RIGHT) || 
                HBInputNative::is_action_just_pressed(HBInputNative::ACTION_ACCEPT)) {
                String val = settings->get(std::string(opt.property.c_str()));
                int idx = opt.choices.find(val);
                if (idx != -1) {
                    idx = (idx + 1) % opt.choices.size();
                } else {
                    idx = 0;
                }
                if (idx >= 0 && idx < (int)opt.choices.size()) {
                    settings->set(std::string(opt.property.c_str()), opt.choices[idx]);
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
    float section_x = 100.0f;
    float section_y = 200.0f;
    float section_w = 300.0f;
    float section_h = 60.0f;
    float section_sep = 10.0f;

    for (size_t i = 0; i < sections.size(); i++) {
        Rect2 rect(section_x, section_y + i * (section_h + section_sep), section_w, section_h);
        Color col = (i == (size_t)selected_section_index) ? Color(0.929f, 0.216f, 0.8f, 0.8f * p_alpha) : Color(0.19f, 0.07f, 0.3f, 0.8f * p_alpha);
        HBVideoDriverPSGL::draw_rect_3d(rect, tilt, col, slant);
        HBVideoDriverPSGL::draw_text_with_font_3d(font_bold, sections[i].name, Vector2(section_x + 20, section_y + i * (section_h + section_sep) + 10), 30, tilt, Color(1, 1, 1, p_alpha));
    }

    // Main panel
    Rect2 panel_rect(450.0f, 150.0f, 1300.0f, 800.0f);
    HBVideoDriverPSGL::draw_rect_3d(panel_rect, tilt, Color(0.19f, 0.07f, 0.3f, 0.85f * p_alpha), slant);

    // Draw options in the selected section
    Section& current_section = sections[selected_section_index];
    float opt_x = 500.0f;
    float opt_y = 200.0f;
    float opt_h = 80.0f;

    for (size_t i = 0; i < current_section.options.size(); i++) {
        Option& opt = current_section.options[i];
        bool is_selected = (i == (size_t)selected_option_index);
        
        if (is_selected) {
            HBVideoDriverPSGL::draw_rect_3d(Rect2(opt_x - 20, opt_y + i * opt_h - 10, 1240, 60), tilt, Color(0.929f, 0.216f, 0.8f, 0.3f * p_alpha), slant);
        }

        Color text_col = is_selected ? Color(1, 1, 1, p_alpha) : Color(0.7f, 0.7f, 0.7f, p_alpha);
        HBVideoDriverPSGL::draw_text_with_font_3d(font, opt.name, Vector2(opt_x, opt_y + i * opt_h), 28, tilt, text_col);
        
        // Value draw
        if (opt.type == String("bool")) {
             bool val = settings->get(std::string(opt.property.c_str()));
             Ref<Image> tex = val ? checkbox_on : checkbox_off;
             if (tex.is_valid()) {
                HBVideoDriverPSGL::draw_texture_3d(tex, Rect2(opt_x + 1000, opt_y + i * opt_h - 5, 40, 40), tilt, text_col);
             }
        } else if (opt.type == String("range")) {
             Variant val = settings->get(std::string(opt.property.c_str()));
             String val_str;
             if (val.get_type() == Variant::FLOAT) {
                 val_str = String::num((double)val, 1);
             } else {
                 val_str = String::num_int64((int64_t)val);
             }
             HBVideoDriverPSGL::draw_text_with_font_3d(font, val_str, Vector2(opt_x + 1000, opt_y + i * opt_h), 28, tilt, text_col);
        } else if (opt.type == String("select")) {
            Variant val = settings->get(std::string(opt.property.c_str()));
            String val_str = val.get_type() == Variant::NIL ? String("N/A") : (String)val;
            String display_str = String("[ < ") + val_str + String(" > ]");
            HBVideoDriverPSGL::draw_text_with_font_3d(font, display_str, Vector2(opt_x + 1000, opt_y + i * opt_h), 28, tilt, text_col, false, true);
        }
    }

    // Footer info
    String footer_info = String("Release Candidate 5 | PS3/CellOS | yt-dlp 2023.03.04");
    HBVideoDriverPSGL::draw_text_with_font_3d(font, footer_info, Vector2(500, 900), 18, tilt, Color(1, 1, 1, 0.5f * p_alpha));
}

void HBOptionsMenuNative::_draw_hint_bar(float p_alpha) {
    Vector2 window_size = HBVideoDriverPSGL::get_window_size();
    float scale_x = window_size.x / 1920.0f;
    float scale_y = window_size.y / 1080.0f;

    HBVideoDriverPSGL::draw_rect(Rect2(0, 1030 * scale_y, window_size.x, 50 * scale_y), Color(0, 0, 0, 0.5f * p_alpha));
    HBVideoDriverPSGL::draw_text_with_font(font, String("Seleccionar: X | Volver: O"), Vector2(100 * scale_x, 1045 * scale_y), (int)(20 * scale_y), Color(1, 1, 1, p_alpha));
}

}
