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

HBRhythmGameNative::HBRhythmGameNative(const Ref<HBSongNative>& p_song) {
    song = p_song;
    judge.instantiate();
    font = ResourceLoader::get_singleton()->load("res://fonts/orbitron/Orbitron-Regular.ttf");
    
    UtilityFunctions::print("HBRhythmGameNative: Starting song: " + (song.is_valid() ? song->get_title() : "NULL"));
    
    if (song.is_valid()) {
        Dictionary charts = song->get_charts();
        if (charts.size() > 0) {
            String chart_name = charts.keys()[0];
            Dictionary chart_data_ref = charts[chart_name];
            String chart_path = song->get_path().path_join((String)chart_data_ref["chart"]);
            
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
    
    // Load textures
    note_textures[HBBaseNoteNative::UP] = ResourceLoader::get_singleton()->load("res://graphics/icons/triangle-normal.svg");
    note_textures[HBBaseNoteNative::DOWN] = ResourceLoader::get_singleton()->load("res://graphics/icons/menu-down.svg");
    note_textures[HBBaseNoteNative::LEFT] = ResourceLoader::get_singleton()->load("res://graphics/icons/menu-left.svg");
    note_textures[HBBaseNoteNative::RIGHT] = ResourceLoader::get_singleton()->load("res://graphics/icons/menu-right.svg");
    note_textures[HBBaseNoteNative::SLIDE_LEFT] = ResourceLoader::get_singleton()->load("res://graphics/icons/arc-counterclockwise.svg");
    note_textures[HBBaseNoteNative::SLIDE_RIGHT] = ResourceLoader::get_singleton()->load("res://graphics/icons/arc-clockwise.svg");
    note_textures[HBBaseNoteNative::HEART] = ResourceLoader::get_singleton()->load("res://graphics/heart.png");

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
    while (next_note_idx < timing_points.size()) {
        Ref<HBBaseNoteNative> note = timing_points[next_note_idx];
        if (note.is_valid()) {
            double note_time = note->get_time() / 1000.0;
            double note_out = note->get_time_out(song->get_bpm()) / 1000.0;
            if (time >= note_time - note_out) {
                active_notes.push_back(note);
                next_note_idx++;
            } else {
                break;
            }
        } else {
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
    
    if (song.is_valid()) {
        HBVideoDriverPSGL::draw_text_with_font(font, song->get_title(), Vector2(50 * scale_x, 50 * scale_y), (int)(40 * scale_y), Color(1, 1, 1, 1));
        HBVideoDriverPSGL::draw_text_with_font(font, "Score: " + String::num_int64(score), Vector2(50 * scale_x, 100 * scale_y), (int)(30 * scale_y), Color(1, 1, 1, 1));
        HBVideoDriverPSGL::draw_text_with_font(font, "Combo: " + String::num_int64(combo), Vector2(50 * scale_x, 140 * scale_y), (int)(30 * scale_y), Color(1, 1, 1, 1));
    }

    // Draw Note Targets
    for (int i = 0; i < 9; i++) {
        // Simple fixed targets for now
        Vector2 pos(960 * scale_x, 540 * scale_y); // Default center
        // In real game, targets are at note.position
    }
    
    // Draw Active Notes
    for (size_t i = 0; i < active_notes.size(); i++) {
        Ref<HBBaseNoteNative> note = active_notes[i];
        double note_time = note->get_time() / 1000.0;
        double note_out = note->get_time_out(song->get_bpm()) / 1000.0;
        
        float progress = (float)((note_time - time) / note_out);
        if (progress < -0.2f) continue;

        Vector2 note_pos = note->get_position();
        Vector2 draw_pos = HBUtilsNative::calculate_note_sine(1.0f - progress, note_pos, note->get_entry_angle(), (float)note->get_oscillation_frequency(), note->get_oscillation_amplitude(), note->get_distance());
        
        draw_pos.x *= scale_x;
        draw_pos.y *= scale_y;

        // Draw Target
        HBVideoDriverPSGL::draw_rect(Rect2(note_pos.x * scale_x - 32 * scale_x, note_pos.y * scale_y - 32 * scale_y, 64 * scale_x, 64 * scale_y), Color(1, 1, 1, 0.3f));

        // Draw Note
        Ref<Image> tex = note_textures[note->get_note_type()];
        if (tex.is_valid()) {
            HBVideoDriverPSGL::draw_texture(tex, Rect2(draw_pos.x - 32 * scale_x, draw_pos.y - 32 * scale_y, 64 * scale_x, 64 * scale_y));
        } else {
            HBVideoDriverPSGL::draw_rect(Rect2(draw_pos.x - 20 * scale_x, draw_pos.y - 20 * scale_y, 40 * scale_x, 40 * scale_y), Color(1, 1, 0, 1));
        }
    }
    
    HBVideoDriverPSGL::draw_text_with_font(font, "Press BACK to return", Vector2(50 * scale_x, 1000 * scale_y), (int)(24 * scale_y), Color(1, 1, 1, 0.5f));
}

}
