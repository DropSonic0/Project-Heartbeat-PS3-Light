#ifndef HB_RHYTHM_GAME_NATIVE_HPP
#define HB_RHYTHM_GAME_NATIVE_HPP

#include "compat/godot_cpp/classes/node.hpp"
#include "chart/hb_song.hpp"
#include "chart/hb_chart.hpp"
#include "hb_judge.hpp"
#include "compat/godot_cpp/classes/font_variation.hpp"
#include "audio/hb_shinobu_playback_offset.hpp"
#include "chart/hb_base_note.hpp"
#include <vector>

namespace godot {

class HBRhythmGameNative : public Node {
    Ref<HBSongNative> song;
    Ref<HBChartNative> chart;
    Ref<HBJudgeNative> judge;
    ShinobuGodotSoundPlaybackOffsetNative* audio_playback = nullptr;
    
    double time = 0.0;
    bool is_playing = false;
    int64_t score = 0;
    int combo = 0;
    int max_combo = 0;
    
    Ref<FontVariation> font;
    Ref<Image> note_textures[9];
    Ref<Image> target_textures[9];

    Array timing_points;
    int next_note_idx = 0;
    std::vector<Ref<HBBaseNoteNative>> active_notes;

    void _process_judgement(HBBaseNoteNative::NoteType p_type);

public:
    HBRhythmGameNative(const Ref<HBSongNative>& p_song);
    ~HBRhythmGameNative();
    
    void update();
    void draw();
};

}

#endif
