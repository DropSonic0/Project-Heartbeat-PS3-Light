#include "hb_song_list_native.hpp"
#include "hb_game.hpp"
#include "utils/hb_input_native.hpp"
#include "graphics/hb_video_driver_psgl.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"
#include "compat/godot_cpp/classes/resource_loader.hpp"

namespace godot {

HBSongListNative::HBSongListNative() {
    font = ResourceLoader::get_singleton()->load("res://fonts/orbitron/Orbitron-Regular.ttf");
    font_bold = ResourceLoader::get_singleton()->load("res://fonts/orbitron/Orbitron-Black.ttf");
    background_tex = ResourceLoader::get_singleton()->load("res://graphics/predarkenedbg.png");
    default_preview = ResourceLoader::get_singleton()->load("res://graphics/no_preview_texture.png");
    arcade_icon = ResourceLoader::get_singleton()->load("res://graphics/resource_packs/fallback/notes/slide_right_note.png");
    console_icon = ResourceLoader::get_singleton()->load("res://graphics/resource_packs/fallback/notes/heart_note.png");
    heart_icon = ResourceLoader::get_singleton()->load("res://graphics/icons/menu_heart.png");
    folder_icon = ResourceLoader::get_singleton()->load("res://graphics/icons/menu_folder.png");
    star_icon = ResourceLoader::get_singleton()->load("res://graphics/icons/menu_star.png");
    filter_icon = ResourceLoader::get_singleton()->load("res://graphics/icons/filter-menu.svg");
    
    prompt_cross = ResourceLoader::get_singleton()->load("res://graphics/resource_packs/playstation/notes/down_note.png");
    prompt_circle = ResourceLoader::get_singleton()->load("res://graphics/resource_packs/playstation/notes/right_note.png");
    prompt_square = ResourceLoader::get_singleton()->load("res://graphics/resource_packs/playstation/notes/left_note.png");
    prompt_triangle = ResourceLoader::get_singleton()->load("res://graphics/resource_packs/playstation/notes/up_note.png");

    Dictionary loaded_songs = HBSongLoaderNative::get_singleton()->get_songs();
    Array keys = loaded_songs.keys();
    for (int i = 0; i < keys.size(); i++) {
        SongEntry entry;
        entry.song = loaded_songs[keys[i]];
        songs.push_back(entry);
    }
    if (!songs.empty()) {
        selected_index = 0;
    }
}

void HBSongListNative::update() {
    HBInputNative::update();

    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_DOWN)) {
        if (!songs.empty()) {
            selected_index = (selected_index + 1) % songs.size();
        }
    }
    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_UP)) {
        if (!songs.empty()) {
            selected_index = (selected_index - 1 + songs.size()) % songs.size();
        }
    }
    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_BACK)) {
        HBGameNative::get_singleton()->change_to_menu("main_menu");
        return;
    }

    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_ACCEPT)) {
        if (selected_index >= 0 && selected_index < (int)songs.size()) {
            HBGameNative::get_singleton()->change_to_menu("rhythm_game", songs[selected_index].song);
            return;
        }
    }
}

Color HBSongListNative::get_difficulty_color(String p_difficulty) {
    String diff = p_difficulty.to_lower();
    if (diff == "easy") return Color::from_hsv(190.0f / 360.0f, 0.7f, 0.75f);
    if (diff == "normal") return Color::from_hsv(111.0f / 360.0f, 0.7f, 0.75f);
    if (diff == "hard") return Color::from_hsv(46.0f / 360.0f, 0.7f, 0.75f);
    if (diff == "extreme") return Color::from_hsv(343.0f / 360.0f, 0.7f, 0.75f);
    return Color(0.5, 0.5, 0.5);
}

static String array_to_string(Array p_array) {
    String res = "";
    for (size_t i = 0; i < p_array.size(); i++) {
        if (i > 0) res += ", ";
        res += (String)p_array[i];
    }
    return res;
}

void HBSongListNative::draw() {
    Vector2 window_size = HBVideoDriverPSGL::get_window_size();
    float scale_x = window_size.x / 1920.0f;
    float scale_y = window_size.y / 1080.0f;

    // Draw background
    if (background_tex.is_valid()) {
        HBVideoDriverPSGL::draw_texture(background_tex, Rect2(0, 0, window_size.x, window_size.y));
    } else {
        HBVideoDriverPSGL::draw_rect(Rect2(0, 0, window_size.x, window_size.y), Color(0.129, 0.071, 0.259, 1.0));
    }

    // Top Left Filter/Sort Icons
    float icon_bar_y = 150 * scale_y;
    HBVideoDriverPSGL::draw_rect(Rect2(20 * scale_x, icon_bar_y, 50 * scale_x, 40 * scale_y), Color(0.3, 0.1, 0.4, 0.7));
    if (filter_icon.is_valid()) {
        HBVideoDriverPSGL::draw_texture(filter_icon, Rect2(30 * scale_x, icon_bar_y + 5 * scale_y, 30 * scale_x, 30 * scale_y));
    }
    
    HBVideoDriverPSGL::draw_rect(Rect2(75 * scale_x, icon_bar_y, 50 * scale_x, 40 * scale_y), Color(0.3, 0.1, 0.4, 0.7));
    HBVideoDriverPSGL::draw_text_with_font(font_bold, "W", Vector2(85 * scale_x, icon_bar_y + 30 * scale_y), (int)(24 * scale_y), Color(1, 1, 1, 1));

    HBVideoDriverPSGL::draw_rect(Rect2(130 * scale_x, icon_bar_y, 100 * scale_x, 40 * scale_y), Color(0.3, 0.1, 0.4, 0.7));
    HBVideoDriverPSGL::draw_text_with_font(font, "Título", Vector2(145 * scale_x, icon_bar_y + 28 * scale_y), (int)(18 * scale_y), Color(1, 1, 1, 1));

    HBVideoDriverPSGL::draw_rect(Rect2(235 * scale_x, icon_bar_y, 140 * scale_x, 40 * scale_y), Color(0.3, 0.1, 0.4, 0.7));
    HBVideoDriverPSGL::draw_text_with_font(font, "Con medios", Vector2(250 * scale_x, icon_bar_y + 28 * scale_y), (int)(18 * scale_y), Color(1, 1, 1, 1));

    // Profile Header (Top Right)
    float profile_x = 1350 * scale_x;
    float profile_y = 70 * scale_y;
    HBVideoDriverPSGL::draw_rect(Rect2(profile_x, profile_y, 500 * scale_x, 100 * scale_y), Color(0.15, 0.05, 0.25, 0.7));
    HBVideoDriverPSGL::draw_text_with_font(font_bold, "DropSonic", Vector2(profile_x + 100 * scale_x, profile_y + 40 * scale_y), (int)(24 * scale_y), Color(1, 1, 1, 1));
    HBVideoDriverPSGL::draw_rect(Rect2(profile_x + 100 * scale_x, profile_y + 55 * scale_y, 300 * scale_x, 10 * scale_y), Color(1, 1, 1, 0.2));
    HBVideoDriverPSGL::draw_rect(Rect2(profile_x + 100 * scale_x, profile_y + 55 * scale_y, 200 * scale_x, 10 * scale_y), Color(0.9, 0.2, 0.8, 1.0)); // Progress bar
    HBVideoDriverPSGL::draw_text_with_font(font, "Nv. 63", Vector2(profile_x + 420 * scale_x, profile_y + 65 * scale_y), (int)(18 * scale_y), Color(1, 1, 1, 0.8));

    // Category Tabs
    const char* tabs[] = {"Todo", "Oficial", "Workshop", "Local", "Carpetas"};
    float tab_x = 850 * scale_x;
    float tab_y = 190 * scale_y;
    for (int i = 0; i < 5; i++) {
        bool is_oficial = (i == 1);
        Color tab_color = is_oficial ? Color(0.93f, 0.22f, 0.8f, 0.8f) : Color(1, 1, 1, 0.6f);
        if (is_oficial) {
            HBVideoDriverPSGL::draw_rect(Rect2(tab_x - 5 * scale_x, tab_y - 25 * scale_y, 80 * scale_x, 35 * scale_y), Color(0.93, 0.22, 0.8, 0.3));
        }
        HBVideoDriverPSGL::draw_text_with_font(font, tabs[i], Vector2(tab_x, tab_y), (int)(24 * scale_y), tab_color);
        tab_x += (i == 1 ? 100 : 120) * scale_x;
    }

    HBVideoDriverPSGL::draw_text_with_font(font_bold, "Juego Libre", Vector2(100 * scale_x, 80 * scale_y), (int)(50 * scale_y), Color(1, 1, 1, 1), true);

    float item_height = 125.0f * scale_y;
    float start_y = 230.0f * scale_y;
    int visible_items = 6;

    if (selected_index >= scroll_offset + visible_items) {
        scroll_offset = selected_index - visible_items + 1;
    }
    if (selected_index < scroll_offset) {
        scroll_offset = selected_index;
    }

    for (int i = 0; i < visible_items && (scroll_offset + i) < (int)songs.size(); i++) {
        int idx = scroll_offset + i;
        float item_y = start_y + i * item_height;
        
        bool is_selected = (idx == selected_index);
        float current_scale = is_selected ? 1.0f : 0.85f;
        
        Color item_color = is_selected ? Color(0.93f, 0.22f, 0.8f, 0.6f) : Color(0.19f, 0.07f, 0.3f, 0.75f);
        Color highlight_color = Color(0.93f, 0.22f, 0.8f, 1.0f);
        
        float draw_h = (item_height - 10 * scale_y) * current_scale;
        float draw_w = 1000 * scale_x * current_scale;
        float offset_y = (item_height - draw_h) / 2.0f;
        float offset_x = 50 * scale_x;
        
        // Selected Glow
        if (is_selected) {
            for (int j = 1; j <= 3; j++) {
                float glow_expand = j * 2.0f * scale_x;
                Color glow_col = highlight_color;
                glow_col.a = 0.15f / j;
                HBVideoDriverPSGL::draw_parallelogram(Rect2(offset_x - glow_expand, item_y + offset_y - glow_expand, draw_w + glow_expand * 2, draw_h + glow_expand * 2), -40.0f * scale_x, glow_col);
            }
        }

        // Main parallelogram
        HBVideoDriverPSGL::draw_parallelogram(Rect2(offset_x, item_y + offset_y, draw_w, draw_h), -40.0f * scale_x * current_scale, item_color);
        
        // Draw Album Art
        Ref<Image> album_art = default_preview;
        
        float art_size = draw_h - 12 * scale_y;
        HBVideoDriverPSGL::draw_texture(album_art, Rect2(offset_x + 10 * scale_x, item_y + offset_y + 6 * scale_y, art_size, art_size));

        // Draw Title and Artist/Creator
        int font_size = (int)(32 * scale_y * current_scale);
        float text_x = offset_x + art_size + 40 * scale_x;
        
        // Heart/Folder Icon
        if (heart_icon.is_valid()) {
            HBVideoDriverPSGL::draw_texture(heart_icon, Rect2(text_x - 35 * scale_x, item_y + offset_y + 15 * scale_y * current_scale, 25 * scale_x * current_scale, 25 * scale_y * current_scale));
        }

        HBVideoDriverPSGL::draw_text_with_font(font_bold, songs[idx].song->get_title(), Vector2(text_x, item_y + offset_y + 42 * scale_y * current_scale), font_size, Color(1, 1, 1, 1), true);
        
        int artist_font_size = (int)(24 * scale_y * current_scale);
        float artist_x = text_x + 500 * scale_x * current_scale; 
        HBVideoDriverPSGL::draw_text_with_font(font, songs[idx].song->get_artist(), Vector2(artist_x, item_y + offset_y + 42 * scale_y * current_scale), artist_font_size, Color(1, 1, 1, 1));

        if (!songs[idx].song->get_creator().is_empty()) {
             HBVideoDriverPSGL::draw_text_with_font(font, songs[idx].song->get_creator(), Vector2(artist_x + 250 * scale_x * current_scale, item_y + offset_y + 42 * scale_y * current_scale), (int)(18 * scale_y * current_scale), Color(0.7, 0.7, 0.7, 1.0));
        }

        // Draw Difficulty Tags
        Dictionary charts = songs[idx].song->get_charts();
        Array chart_keys = charts.keys();
        // Simple sorting for common difficulties
        Array sorted_keys;
        const char* diff_order[] = {"Easy", "Normal", "Hard", "Extreme"};
        for (int d = 0; d < 4; d++) {
            if (charts.has(diff_order[d])) {
                sorted_keys.push_back(diff_order[d]);
            }
        }

        float tag_x = text_x;
        float tag_y = item_y + offset_y + 65 * scale_y * current_scale;
        float tag_h = 35 * scale_y * current_scale;
        int tag_font_size = (int)(20 * scale_y * current_scale);

        for (int k = 0; k < sorted_keys.size(); k++) {
            String diff_name = sorted_keys[k];
            Dictionary chart_data = charts[diff_name];
            float stars = 0.0f;
            if (chart_data.has("stars")) {
                stars = (float)chart_data["stars"];
            }

            Color diff_color = get_difficulty_color(diff_name);
            String star_text = String::num(stars);
            if (stars == (int)stars) {
                star_text = String::num_int64((int64_t)stars);
            }
            
            // Draw star/number part
            float star_w = 45 * scale_x * current_scale;
            HBVideoDriverPSGL::draw_parallelogram(Rect2(tag_x, tag_y, star_w, tag_h), -5.0f * scale_x * current_scale, Color(0.1, 0.1, 0.1, 0.8));
            HBVideoDriverPSGL::draw_text_with_font(font, star_text, Vector2(tag_x + 5 * scale_x, tag_y + tag_h * 0.75f), tag_font_size, Color(1, 1, 1, 1));
            
            // Draw difficulty name part
            float name_w = 110 * scale_x * current_scale;
            HBVideoDriverPSGL::draw_parallelogram(Rect2(tag_x + star_w + 2 * scale_x, tag_y, name_w, tag_h), -5.0f * scale_x * current_scale, diff_color);
            HBVideoDriverPSGL::draw_text_with_font(font_bold, diff_name.to_upper(), Vector2(tag_x + star_w + 10 * scale_x, tag_y + tag_h * 0.75f), tag_font_size, Color(1, 1, 1, 1));
            
            tag_x += star_w + name_w + 10 * scale_x * current_scale;
        }

        // Draw Note Usage Icons
        float icon_size = 35 * scale_y * current_scale;
        float icon_x = text_x - 45 * scale_x * current_scale;
        
        if (arcade_icon.is_valid()) {
            HBVideoDriverPSGL::draw_texture(arcade_icon, Rect2(icon_x, item_y + offset_y + 10 * scale_y * current_scale, icon_size, icon_size));
            icon_x += 40 * scale_x * current_scale;
        }
        if (console_icon.is_valid()) {
            HBVideoDriverPSGL::draw_texture(console_icon, Rect2(icon_x, item_y + offset_y + 10 * scale_y * current_scale, icon_size, icon_size));
        }
    }
    
    // Scrollbar
    float scrollbar_h = visible_items * item_height;
    HBVideoDriverPSGL::draw_rect(Rect2(1060 * scale_x, start_y, 10 * scale_x, scrollbar_h), Color(1, 1, 1, 0.1));
    if (!songs.empty()) {
        float handle_h = scrollbar_h * (visible_items / (float)songs.size());
        if (handle_h > scrollbar_h) handle_h = scrollbar_h;
        float handle_y = start_y + (scroll_offset / (float)songs.size()) * scrollbar_h;
        HBVideoDriverPSGL::draw_rect(Rect2(1060 * scale_x, handle_y, 10 * scale_x, handle_h), Color(1, 1, 1, 0.8));
    }

    // Song Counter
    String counter_text = String::num_int64(selected_index + 1) + "/" + String::num_int64(songs.size());
    HBVideoDriverPSGL::draw_text_with_font(font, counter_text, Vector2(50 * scale_x, 1000 * scale_y), (int)(30 * scale_y), Color(1, 1, 1, 1), true);

    // Bottom Hints Bar
    float hint_bar_w = 900 * scale_x;
    float hint_bar_x = (window_size.x - hint_bar_w) / 2.0f - 100 * scale_x;
    HBVideoDriverPSGL::draw_parallelogram(Rect2(hint_bar_x, 960 * scale_y, hint_bar_w, 60 * scale_y), -20.0f * scale_x, Color(0.1, 0.05, 0.2, 0.9));
    float hint_x = hint_bar_x + 30 * scale_x;
    float hint_y = 1000 * scale_y;
    
    auto draw_hint = [&](const char* label, Ref<Image> button_tex, float label_w, float spacing = 180.0f) {
        HBVideoDriverPSGL::draw_text_with_font(font, label, Vector2(hint_x, hint_y), (int)(24 * scale_y), Color(1, 1, 1, 1));
        if (button_tex.is_valid()) {
            HBVideoDriverPSGL::draw_texture(button_tex, Rect2(hint_x + label_w, hint_y - 40 * scale_y, 45 * scale_x, 45 * scale_y));
        } else {
             HBVideoDriverPSGL::draw_rect(Rect2(hint_x + label_w, hint_y - 32 * scale_y, 45 * scale_x, 45 * scale_y), Color(1, 1, 1, 0.9));
        }
        hint_x += spacing * scale_x;
    };

    draw_hint("Buscar", prompt_square, 100 * scale_x, 180.0f);
    draw_hint("Añadir a...", prompt_triangle, 140 * scale_x, 220.0f);
    draw_hint("Seleccionar", prompt_circle, 150 * scale_x, 250.0f);
    draw_hint("Volver", prompt_cross, 100 * scale_x);

    // Draw "preview" for selected song
    if (selected_index >= 0 && selected_index < (int)songs.size()) {
        if (loaded_preview_idx != selected_index) {
            String preview_path = songs[selected_index].song->get_song_preview_res_path();
            if (!preview_path.is_empty()) {
                current_preview = ResourceLoader::get_singleton()->load(preview_path);
            } else {
                current_preview.unref();
            }
            loaded_preview_idx = selected_index;
        }
        
        Ref<Image> preview = current_preview;
        if (preview.is_null()) {
            preview = default_preview;
        }
        
        float preview_x = 1350 * scale_x;
        float preview_y = 350 * scale_y;
        float preview_w = 480 * scale_x;
        float preview_h = 480 * scale_y;
        
        // Preview Background and Art
        HBVideoDriverPSGL::draw_rect(Rect2(preview_x - 10 * scale_x, preview_y - 10 * scale_y, preview_w + 20 * scale_x, preview_h + 20 * scale_y), Color(0.1, 0.1, 0.1, 0.5));
        HBVideoDriverPSGL::draw_texture(preview, Rect2(preview_x, preview_y, preview_w, preview_h));
        
        // Song Info on the right
        float info_x = 1350 * scale_x;
        float info_y = 200 * scale_y;
        
        HBVideoDriverPSGL::draw_text_with_font(font_bold, songs[selected_index].song->get_title(), Vector2(info_x, info_y), (int)(42 * scale_y), Color(1, 1, 1, 1), true, true);
        HBVideoDriverPSGL::draw_text_with_font(font, "por", Vector2(info_x + 200 * scale_x, info_y + 40 * scale_y), (int)(20 * scale_y), Color(1, 1, 1, 0.8));
        HBVideoDriverPSGL::draw_text_with_font(font_bold, songs[selected_index].song->get_artist(), Vector2(info_x + 50 * scale_x, info_y + 80 * scale_y), (int)(28 * scale_y), Color(1, 1, 1, 1));
        
        // BPM and other details
        float detail_y = 750 * scale_y;
        HBVideoDriverPSGL::draw_rect(Rect2(info_x - 40 * scale_x, detail_y - 20 * scale_y, 550 * scale_x, 220 * scale_y), Color(0.1, 0.05, 0.2, 0.6));
        
        String bpm_text = String::num(songs[selected_index].song->get_bpm()) + " BPM";
        HBVideoDriverPSGL::draw_text_with_font(font, bpm_text, Vector2(info_x + 150 * scale_x, detail_y + 30 * scale_y), (int)(24 * scale_y), Color(1, 1, 1, 1));
        
        String writers_str = array_to_string(songs[selected_index].song->get_writers());
        if (writers_str.is_empty()) writers_str = "-";
        HBVideoDriverPSGL::draw_text_with_font(font, "Escrito por: " + writers_str, Vector2(info_x, detail_y + 70 * scale_y), (int)(20 * scale_y), Color(1, 1, 1, 0.8));
        
        String vocals_str = array_to_string(songs[selected_index].song->get_vocals());
        if (vocals_str.is_empty()) vocals_str = "-";
        HBVideoDriverPSGL::draw_text_with_font(font, "Vocales por: " + vocals_str, Vector2(info_x, detail_y + 110 * scale_y), (int)(20 * scale_y), Color(1, 1, 1, 0.8));

        // Mini player (Bottom Right)
        float player_w = 600 * scale_x;
        float player_x = window_size.x - player_w - 20 * scale_x;
        float player_y = 950 * scale_y;
        HBVideoDriverPSGL::draw_parallelogram(Rect2(player_x, player_y, player_w, 100 * scale_y), -40.0f * scale_x, Color(0.15, 0.08, 0.25, 0.9));
        if (preview.is_valid()) {
            HBVideoDriverPSGL::draw_texture(preview, Rect2(player_x + 40 * scale_x, player_y + 10 * scale_y, 80 * scale_y, 80 * scale_y));
        }
        HBVideoDriverPSGL::draw_text_with_font(font_bold, songs[selected_index].song->get_title(), Vector2(player_x + 140 * scale_x, player_y + 45 * scale_y), (int)(24 * scale_y), Color(1, 1, 1, 1));
        HBVideoDriverPSGL::draw_rect(Rect2(player_x + 140 * scale_x, player_y + 65 * scale_y, 420 * scale_x, 10 * scale_y), Color(1, 1, 1, 0.2));
        HBVideoDriverPSGL::draw_rect(Rect2(player_x + 140 * scale_x, player_y + 65 * scale_y, 200 * scale_x, 10 * scale_y), Color(1, 1, 1, 1.0)); // Playback bar
        HBVideoDriverPSGL::draw_text_with_font(font, "00:51", Vector2(player_x + 140 * scale_x, player_y + 95 * scale_y), (int)(18 * scale_y), Color(1, 1, 1, 0.8));
        HBVideoDriverPSGL::draw_text_with_font(font, "02:44", Vector2(player_x + 520 * scale_x, player_y + 95 * scale_y), (int)(18 * scale_y), Color(1, 1, 1, 0.8));
    }
}

}
