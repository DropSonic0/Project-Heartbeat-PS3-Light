#include "hb_rhythm_game_native.hpp"
#include "hb_game.hpp"
#include "graphics/hb_video_driver_psgl.hpp"
#include "utils/hb_input_native.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"
#include "compat/godot_cpp/classes/resource_loader.hpp"
#include "compat/godot_cpp/classes/file_access.hpp"
#include "compat/godot_cpp/classes/json.hpp"
#include "chart/hb_timing_point.hpp"
#include "utils/hb_utils.hpp"

namespace godot {

HBRhythmGameNative::HBRhythmGameNative(const Variant& p_params) {
    String selected_chart = "";
    if (p_params.get_type() == Variant::DICTIONARY) {
        Dictionary d = p_params;
        if (d.has("song")) {
            song = d["song"];
        }
        if (d.has("difficulty")) {
            selected_chart = d["difficulty"];
        }
    } else if (p_params.get_type() == Variant::OBJECT) {
        song = p_params;
    }

    judge.instantiate();
    font = ResourceLoader::get_singleton()->load("res://fonts/orbitron/Orbitron-Regular.ttf");
    
    UtilityFunctions::print("HBRhythmGameNative: Starting song: " + (song.is_valid() ? song->get_title() : "NULL") + " Difficulty: " + selected_chart);
    
    if (song.is_valid()) {
        Dictionary charts = song->get_charts();
        if (charts.size() > 0) {
            String chart_name = selected_chart;
            if (chart_name.is_empty() || !charts.has(chart_name)) {
                chart_name = charts.keys()[0];
            }
            Dictionary chart_data_ref = charts[chart_name];
            String chart_path = song->get_path().path_join((String)chart_data_ref["chart"]);
            UtilityFunctions::print("HBRhythmGameNative: Loading chart from: " + chart_path);
            
            Ref<FileAccess> fa = FileAccess::open(chart_path, FileAccess::READ);
            if (fa.is_valid()) {
                String content = fa->get_as_text();
                Ref<JSON> json;
                json.instantiate();
                if (json->parse(content) == 0) {
                    chart.instantiate();
                    chart->deserialize(json->get_data(), song);
                    UtilityFunctions::print("HBRhythmGameNative: Chart loaded successfully.");
                    timing_points = chart->get_timing_points();
                }
            }
        }
        
        audio_playback = memnew(ShinobuGodotSoundPlaybackOffsetNative);
        String audio_path = song->get_song_audio_res_path();
        // Since we are in PS3 Light shim, audio playback might be limited.
        // For now we use the offset logic but it might not play real audio yet.
        audio_playback->start();
    }
    
    Ref<HBUserSettingsNative> settings = HBGameNative::get_singleton()->get_user_settings();
    String icon_pack = settings->get(std::string("icon_pack"));
    String pack_path = String("res://graphics/resource_packs/").path_join(icon_pack).path_join(String("notes/"));

    // Load textures
    note_textures[HBBaseNoteNative::UP] = ResourceLoader::get_singleton()->load(pack_path.path_join(String("up_note.svg")));
    note_textures[HBBaseNoteNative::DOWN] = ResourceLoader::get_singleton()->load(pack_path.path_join(String("down_note.svg")));
    note_textures[HBBaseNoteNative::LEFT] = ResourceLoader::get_singleton()->load(pack_path.path_join(String("left_note.svg")));
    note_textures[HBBaseNoteNative::RIGHT] = ResourceLoader::get_singleton()->load(pack_path.path_join(String("right_note.svg")));
    note_textures[HBBaseNoteNative::SLIDE_LEFT] = ResourceLoader::get_singleton()->load(String("res://graphics/icons/arc-counterclockwise.svg"));
    note_textures[HBBaseNoteNative::SLIDE_RIGHT] = ResourceLoader::get_singleton()->load(String("res://graphics/icons/arc-clockwise.svg"));
    note_textures[HBBaseNoteNative::HEART] = ResourceLoader::get_singleton()->load(String("res://graphics/icons/menu_heart_white.svg"));

    // Fallbacks if icon pack is missing notes
    if (note_textures[HBBaseNoteNative::UP].is_null()) note_textures[HBBaseNoteNative::UP] = ResourceLoader::get_singleton()->load("res://graphics/icons/triangle-normal.svg");
    if (note_textures[HBBaseNoteNative::DOWN].is_null()) note_textures[HBBaseNoteNative::DOWN] = ResourceLoader::get_singleton()->load("res://graphics/icons/menu-down.svg");
    if (note_textures[HBBaseNoteNative::LEFT].is_null()) note_textures[HBBaseNoteNative::LEFT] = ResourceLoader::get_singleton()->load("res://graphics/icons/menu-left.svg");
    if (note_textures[HBBaseNoteNative::RIGHT].is_null()) note_textures[HBBaseNoteNative::RIGHT] = ResourceLoader::get_singleton()->load("res://graphics/icons/menu-right.svg");

    health_bar_tex = ResourceLoader::get_singleton()->load("res://graphics/heart_power_bar.png");
    health_bar_bg_tex = ResourceLoader::get_singleton()->load("res://graphics/heart_power_bar_bg.png");
    clear_bar_tex = ResourceLoader::get_singleton()->load("res://graphics/clear_bar.png");
    clear_bar_postclear_tex = ResourceLoader::get_singleton()->load("res://graphics/clear_bar_postclear.png");

    if (song.is_valid()) {
        String bg_img = song->get_background_image();
        if (!bg_img.is_empty()) {
            String path = song->get_path().path_join(bg_img);
            UtilityFunctions::print("HBRhythmGameNative: Loading background from: " + path);
            background_texture = ResourceLoader::get_singleton()->load(path);
        }
    }

    is_playing = true;
}

HBRhythmGameNative::~HBRhythmGameNative() {
    if (audio_playback) {
        audio_playback->stop();
        memdelete(audio_playback);
    }
}

void HBRhythmGameNative::update() {
    HBInputNative::update();
    
    if (is_playing && audio_playback) {
        time = audio_playback->get_playback_position_msec() / 1000.0;
    }
    
    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_BACK)) {
        HBGameNative::get_singleton()->change_to_menu("song_list");
        return;
    }

    // Process song end
    if (next_note_idx >= timing_points.size() && active_notes.empty()) {
        // Song finished!
        // HBGameNative::get_singleton()->change_to_menu("song_list");
    }

    // Activate new notes
    while (next_note_idx < (int)timing_points.size()) {
        Variant v = timing_points[next_note_idx];
        Ref<HBBaseNoteNative> note;
        if (v.get_type() == Variant::OBJECT) {
            note = v;
        }

        if (note.is_valid()) {
            double note_time = note->get_time() / 1000.0;
            double note_out = note->get_time_out(song->get_bpm()) / 1000.0;
            if (time >= note_time - note_out) {
                active_notes.push_back(note);
                next_note_idx++;
            } else {
                // Not time yet for this note, and since they are ordered, we stop here
                break;
            }
        } else {
            // Skip invalid objects in timing points
            next_note_idx++;
        }
    }

    // Handle Input Judgement
    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_UP)) _process_judgement(HBBaseNoteNative::UP);
    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_DOWN)) _process_judgement(HBBaseNoteNative::DOWN);
    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_LEFT)) _process_judgement(HBBaseNoteNative::LEFT);
    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_RIGHT)) _process_judgement(HBBaseNoteNative::RIGHT);
    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_SQUARE)) _process_judgement(HBBaseNoteNative::LEFT); // Simplified mapping
    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_TRIANGLE)) _process_judgement(HBBaseNoteNative::UP);
    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_ACCEPT)) _process_judgement(HBBaseNoteNative::DOWN);

    // Process Misses
    for (int i = (int)active_notes.size() - 1; i >= 0; i--) {
        Ref<HBBaseNoteNative> note = active_notes[i];
        double note_time = note->get_time() / 1000.0;
        if (time > note_time + 0.2) { // 200ms grace period for MISS
            active_notes.erase(active_notes.begin() + i);
            combo = 0;
            
            // Health removal on MISS
            int fail_reduction = (fail_combo < 4) ? fail_combo : 4;
            health -= (5.0f - (float)fail_reduction);
            if (health < 0.0f) health = 0.0f;
            fail_combo++;

            UtilityFunctions::print("MISS!");
        }
    }
}

void HBRhythmGameNative::_process_judgement(HBBaseNoteNative::NoteType p_type) {
    for (int i = 0; i < (int)active_notes.size(); i++) {
        Ref<HBBaseNoteNative> note = active_notes[i];
        if (note->get_note_type() == p_type) {
            int64_t hit_time = (int64_t)(time * 1000.0);
            int64_t target_time = note->get_time();
            int rating = judge->judge_note(hit_time, target_time);
            
            if (rating != HBJudgeNative::WORST) {
                score += note->get_score(rating);
                combo++;
                if (combo > max_combo) max_combo = combo;
                last_rating = rating;
                last_rating_time = time;
                
                // Health management
                if (rating >= HBJudgeNative::FINE) {
                    health += 1.0f;
                    if (health > 100.0f) health = 100.0f;
                    fail_combo = 0;
                } else {
                    int fail_reduction = (fail_combo < 4) ? fail_combo : 4;
                    health -= (5.0f - (float)fail_reduction);
                    if (health < 0.0f) health = 0.0f;
                    fail_combo++;
                    combo = 0; // Combo break on SAD/SAFE in most modes
                }

                active_notes.erase(active_notes.begin() + i);
                // Play SFX? 
                return;
            }
        }
    }
}

void HBRhythmGameNative::draw() {
    Vector2 window_size = HBVideoDriverPSGL::get_window_size();
    float scale_x = window_size.x / 1920.0f;
    float scale_y = window_size.y / 1080.0f;

    HBVideoDriverPSGL::clear_buffer();
    
    // Draw Background
    if (background_texture.is_valid()) {
        HBVideoDriverPSGL::draw_texture(background_texture, Rect2(0, 0, window_size.x, window_size.y), Color(1, 1, 1, 0.8f));
    }

    if (song.is_valid()) {
        // Top HUD Panel (Slanted)
        HBVideoDriverPSGL::draw_parallelogram(Rect2(0, 0, window_size.x, 140 * scale_y), -35.0f * scale_x, Color(0, 0, 0, 0.6f));

        // Song Title (Top Left)
        HBVideoDriverPSGL::draw_text_with_font(font, song->get_title(), Vector2(80 * scale_x, 35 * scale_y), (int)(42 * scale_y), Color(1, 1, 1, 1), true);
        
        // Score (Top Left, below title)
        HBVideoDriverPSGL::draw_text_with_font(font, String::num_int64(score), Vector2(80 * scale_x, 85 * scale_y), (int)(34 * scale_y), Color(1, 1, 1, 1), true);

        // Clear Bar (Magic Bar) - Top Center
        float cb_w = 600.0f * scale_x;
        float cb_h = 20.0f * scale_y;
        float cb_x = (window_size.x - cb_w) / 2.0f;
        float cb_y = 100 * scale_y;

        if (clear_bar_tex.is_valid()) {
            HBVideoDriverPSGL::draw_texture(clear_bar_tex, Rect2(cb_x, cb_y, cb_w, cb_h), Color(1, 1, 1, 0.5f));
            double max_score = chart.is_valid() ? chart->get_max_score() : 1.0;
            float progress = (float)((double)score / (max_score > 0 ? max_score : 1.0));
            if (progress > 1.0f) progress = 1.0f;
            
            Ref<Image> fill_tex = (progress >= 0.8f) ? clear_bar_postclear_tex : clear_bar_tex;
            if (fill_tex.is_valid()) {
                HBVideoDriverPSGL::draw_texture(fill_tex, Rect2(cb_x, cb_y, cb_w * progress, cb_h));
            }
        }

        // Percentage display (Top Right)
        float percentage = 0.0f;
        if (timing_points.size() > 0) {
            percentage = (float)next_note_idx / (float)timing_points.size() * 100.0f;
        }
        HBVideoDriverPSGL::draw_text_with_font(font, String::num(percentage) + "%", Vector2(window_size.x - 220 * scale_x, 50 * scale_y), (int)(36 * scale_y), Color(1, 1, 1, 1), true);

        // Bottom HUD Panel (Slanted)
        HBVideoDriverPSGL::draw_parallelogram(Rect2(0, window_size.y - 100 * scale_y, window_size.x, 100 * scale_y), 35.0f * scale_x, Color(0, 0, 0, 0.6f));

        // Health Bar (Bottom Left)
        float hb_w = 450.0f * scale_x;
        float hb_h = 30.0f * scale_y;
        float hb_x = 100 * scale_x;
        float hb_y = window_size.y - 65 * scale_y;

        if (health_bar_bg_tex.is_valid()) {
            HBVideoDriverPSGL::draw_texture(health_bar_bg_tex, Rect2(hb_x, hb_y, hb_w, hb_h));
        }

        float health_w = hb_w * (health / 100.0f);
        if (health_bar_tex.is_valid()) {
            HBVideoDriverPSGL::draw_texture(health_bar_tex, Rect2(hb_x, hb_y, health_w, hb_h));
        } else {
            Color h_col = Color(0, 1, 0, 1);
            if (health < 25.0f) h_col = Color(1, 0, 0, 1);
            else if (health < 50.0f) h_col = Color(1, 1, 0, 1);
            HBVideoDriverPSGL::draw_rect(Rect2(hb_x, hb_y, health_w, hb_h), h_col);
        }
    }

    // Combo (Center Bottom)
    if (combo > 0) {
        HBVideoDriverPSGL::draw_text_with_font(font, "COMBO", Vector2(window_size.x / 2.0f, window_size.y - 160 * scale_y), (int)(24 * scale_y), Color(1, 1, 1, 0.8f), true, true);
        HBVideoDriverPSGL::draw_text_with_font(font, String::num_int64(combo), Vector2(window_size.x / 2.0f, window_size.y - 120 * scale_y), (int)(48 * scale_y), Color(1, 1, 1, 1), true, true);
    }

    // Draw Upcoming Note Targets
    for (int i = next_note_idx; i < (int)timing_points.size(); i++) {
        Variant v = timing_points[i];
        Ref<HBBaseNoteNative> note;
        if (v.get_type() == Variant::OBJECT) {
            note = v;
        }
        if (note.is_null()) continue;
        
        double note_time = note->get_time() / 1000.0;
        if (note_time > time + 3.0) break; // Next 3 seconds
        
        Vector2 note_pos = note->get_position();
        // Use a semi-transparent target texture or rect with better styling
        HBVideoDriverPSGL::draw_rect(Rect2(note_pos.x * scale_x - 45 * scale_x, note_pos.y * scale_y - 45 * scale_y, 90 * scale_x, 90 * scale_y), Color(1, 1, 1, 0.15f), true);
    }
    
    // Draw Active Notes and their targets
    for (size_t i = 0; i < active_notes.size(); i++) {
        Ref<HBBaseNoteNative> note = active_notes[i];
        double note_time = note->get_time() / 1000.0;
        double note_out = note->get_time_out(song->get_bpm()) / 1000.0;
        
        float progress = (float)((note_time - time) / note_out);
        if (progress < -0.2f) continue;

        Vector2 note_pos = note->get_position();
        Vector2 draw_pos = HBUtilsNative::calculate_note_sine(1.0f - progress, note_pos, note->get_entry_angle(), (float)note->get_oscillation_frequency(), note->get_oscillation_amplitude(), note->get_distance());
        
        float dx = draw_pos.x * scale_x;
        float dy = draw_pos.y * scale_y;
        float nx = note_pos.x * scale_x;
        float ny = note_pos.y * scale_y;

        // Draw Target (More opaque for active notes, additive)
        HBVideoDriverPSGL::draw_rect(Rect2(nx - 45 * scale_x, ny - 45 * scale_y, 90 * scale_x, 90 * scale_y), Color(1, 1, 1, 0.4f), true);

        // Draw Note
        Ref<Image> tex = note_textures[note->get_note_type()];
        if (tex.is_valid()) {
            HBVideoDriverPSGL::draw_texture(tex, Rect2(dx - 40 * scale_x, dy - 40 * scale_y, 80 * scale_x, 80 * scale_y));
        } else {
            HBVideoDriverPSGL::draw_rect(Rect2(dx - 30 * scale_x, dy - 30 * scale_y, 60 * scale_x, 60 * scale_y), Color(1, 1, 0, 1));
        }
    }
    
    // Draw Rating
    if (last_rating != -1 && time < last_rating_time + 0.5) {
        String rating_text = "";
        Color rating_color = Color(1, 1, 1);
        switch (last_rating) {
            case 4: rating_text = "COOL"; rating_color = Color(0, 1, 1); break;
            case 3: rating_text = "FINE"; rating_color = Color(0, 1, 0); break;
            case 2: rating_text = "SAFE"; rating_color = Color(1, 1, 0); break;
            case 1: rating_text = "SAD"; rating_color = Color(1, 0, 1); break;
        }
        // Additive rating for "glow" effect
        HBVideoDriverPSGL::draw_text_with_font(font, rating_text, Vector2(window_size.x / 2.0f, 800 * scale_y), (int)(60 * scale_y), rating_color, true, true);
    }

    // Bottom progress bar
    if (timing_points.size() > 0) {
        float progress_val = (float)next_note_idx / (float)timing_points.size();
        HBVideoDriverPSGL::draw_rect(Rect2(0, window_size.y - 10 * scale_y, window_size.x * progress_val, 10 * scale_y), Color(0, 0.8, 1, 0.7f), true);
    }

    HBVideoDriverPSGL::draw_text_with_font(font, "Press BACK to return", Vector2(50 * scale_x, window_size.y - 30 * scale_y), (int)(20 * scale_y), Color(1, 1, 1, 0.5f));
}

}
