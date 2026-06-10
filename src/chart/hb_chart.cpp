#include "hb_chart.hpp"
#include "hb_timing_point.hpp"
#include "hb_note_data.hpp"
#include "hb_duration_note_data.hpp"
#include "hb_sustain_note.hpp"
#include "hb_double_note.hpp"
#include "hb_rush_note.hpp"
#include "hb_timing_change.hpp"
#include "hb_bpm_change.hpp"
#include "hb_chart_section.hpp"
#include "hb_intro_skip_marker.hpp"
#include "hb_song.hpp"
#include "hb_ppd_song.hpp"
#include "hb_auto_song.hpp"
#include "hb_result.hpp"
#include "hb_game_info.hpp"
#include "hb_per_song_settings.hpp"
#include "hb_per_song_editor_settings.hpp"
#include "hb_song_stats.hpp"
#include "hb_song_variant_data.hpp"
#include "hb_folder.hpp"
#include "hb_user_settings.hpp"
#include "hb_web_user_info.hpp"
#include "hb_editor_template.hpp"
#include "hb_history_entry.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm>

using namespace godot;

void HBChartNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_timing_points", "ordered"), &HBChartNative::get_timing_points, DEFVAL(true));
    ClassDB::bind_method(D_METHOD("serialize"), &HBChartNative::serialize);
    ClassDB::bind_method(D_METHOD("deserialize", "data", "song"), &HBChartNative::deserialize);
    ClassDB::bind_method(D_METHOD("get_max_score"), &HBChartNative::get_max_score);
    ClassDB::bind_static_method("HBChartNative", D_METHOD("get_slide_hold_chains_for_points", "timing_points"), &HBChartNative::get_slide_hold_chains_for_points);
    ClassDB::bind_method(D_METHOD("get_slide_hold_chains"), &HBChartNative::get_slide_hold_chains);
    ClassDB::bind_method(D_METHOD("get_layer_i", "layer_name"), &HBChartNative::get_layer_i);

    ClassDB::bind_method(D_METHOD("_note_comparison_internal", "a", "b"), &HBChartNative::_note_comparison_internal);
}

HBChartNative::HBChartNative() {
    populate_layers();
}

HBChartNative::~HBChartNative() {}

void HBChartNative::populate_layers() {
    layers.clear();
    static const char* initial_layers[] = {
        "NoteLeft", "NoteDown", "NoteUp", "NoteRight",
        "SlideLeft", "SlideRight", "Heart", "Events"
    };
    for (const char* name : initial_layers) {
        Dictionary layer;
        layer["name"] = String(name);
        layer["timing_points"] = Array();
        layers.append(layer);
    }
}

bool HBChartNative::_note_comparison_internal(const Variant &a, const Variant &b) {
    return (double)a.get("time") < (double)b.get("time");
}

Array HBChartNative::get_timing_points(bool p_ordered) {
    Array points;
    for (int i = 0; i < layers.size(); i++) {
        Dictionary layer = layers[i];
        Array items = layer["timing_points"];
        points.append_array(items);
    }
    if (p_ordered) {
        points.sort_custom(Callable(this, "_note_comparison_internal"));
    }
    return points;
}

Dictionary HBChartNative::serialize() {
    Dictionary res;
    Array serialized_layers;
    for (int i = 0; i < layers.size(); i++) {
        Dictionary layer = layers[i];
        Array timing_points = layer["timing_points"];
        Array serialized_points;
        for (int j = 0; j < timing_points.size(); j++) {
            Variant point = timing_points[j];
            if (point.get_type() == Variant::OBJECT) {
                serialized_points.append(point.call("serialize"));
            } else {
                serialized_points.append(point);
            }
        }
        Dictionary l;
        l["name"] = layer["name"];
        l["timing_points"] = serialized_points;
        serialized_layers.append(l);
    }
    res["layers"] = serialized_layers;
    res["format_version"] = format_version;
    return res;
}

static Ref<HBSerializableNative> _factory(const String &p_type) {
    if (p_type == "Note") {
        return memnew(HBNoteDataNative);
    } else if (p_type == "TimingPoint") {
        return memnew(HBTimingPointNative);
    } else if (p_type == "BaseNote") {
        return memnew(HBBaseNoteNative);
    } else if (p_type == "DurationNote") {
        return memnew(HBDurationNoteDataNative);
    } else if (p_type == "SustainNote") {
        return memnew(HBSustainNoteNative);
    } else if (p_type == "DoubleNote") {
        return memnew(HBDoubleNoteNative);
    } else if (p_type == "RushNote") {
        return memnew(HBRushNoteNative);
    } else if (p_type == "TimingChange") {
        return memnew(HBTimingChangeNative);
    } else if (p_type == "BpmChange") {
        return memnew(HBBPMChangeNative);
    } else if (p_type == "ChartSection") {
        return memnew(HBChartSectionNative);
    } else if (p_type == "IntroSkipMarker") {
        return memnew(HBIntroSkipMarkerNative);
    } else if (p_type == "Song") {
        return memnew(HBSongNative);
    } else if (p_type == "PPDSong") {
        return memnew(HBPPDSongNative);
    } else if (p_type == "AutoSong") {
        return memnew(HBAutoSongNative);
    } else if (p_type == "Result") {
        return memnew(HBResultNative);
    } else if (p_type == "GameInfo") {
        return memnew(HBGameInfoNative);
    } else if (p_type == "PerSongSettings") {
        return memnew(HBPerSongSettingsNative);
    } else if (p_type == "PerSongEditorSettings") {
        return memnew(HBPerSongEditorSettingsNative);
    } else if (p_type == "SongStats") {
        return memnew(HBSongStatsNative);
    } else if (p_type == "HBSongVariantData") {
        return memnew(HBSongVariantDataNative);
    } else if (p_type == "Folder") {
        return memnew(HBFolderNative);
    } else if (p_type == "UserSettings") {
        return memnew(HBUserSettingsNative);
    } else if (p_type == "WebUserInfo") {
        return memnew(HBWebUserInfoNative);
    } else if (p_type == "EditorTemplate") {
        return memnew(HBEditorTemplateNative);
    } else if (p_type == "HBHistoryEntry") {
        return memnew(HBHistoryEntryNative);
    }
    return nullptr;
}

void HBChartNative::deserialize(Dictionary p_data, Variant p_song) {
    if (p_data.has("layers")) {
        Array data_layers = p_data["layers"];
        for (int i = 0; i < data_layers.size(); i++) {
            Dictionary layer_data = data_layers[i];
            String layer_name = layer_data["name"];
            Array points_data = layer_data["timing_points"];

            Array timing_points;
            for (int j = 0; j < points_data.size(); j++) {
                Dictionary point_dict = points_data[j];
                if (point_dict.has("type")) {
                    Ref<HBSerializableNative> obj = _factory(point_dict["type"]);
                    if (obj.is_valid()) {
                        // In a full implementation, we'd have HBSerializableNative::deserialize_from_dict
                        // For now we set properties manually or assume the object can handle it.
                        // Here we just store the Ref.
                        Array keys = point_dict.keys();
                        for (int k = 0; k < keys.size(); k++) {
                            String key = keys[k];
                            if (key != "type") {
                                obj->set(key, point_dict[key]);
                            }
                        }
                        timing_points.append(obj);
                    } else {
                        timing_points.append(point_dict);
                    }
                } else {
                    timing_points.append(point_dict);
                }
            }

            int idx = get_layer_i(layer_name);
            if (idx != -1) {
                Dictionary l = layers[idx];
                l["timing_points"] = timing_points;
            } else {
                Dictionary l;
                l["name"] = layer_name;
                l["timing_points"] = timing_points;
                layers.append(l);
            }
        }
    }
    if (p_data.has("format_version")) {
        format_version = p_data["format_version"];
    }
}

double HBChartNative::get_max_score() {
    Array tp = get_timing_points();
    double max_score = 0.0;
    return max_score;
}

Dictionary HBChartNative::get_slide_hold_chains_for_points(Array p_timing_points) {
    Dictionary slide_hold_chains;
    return slide_hold_chains;
}

Dictionary HBChartNative::get_slide_hold_chains() {
    Dictionary slide_hold_chains;
    return slide_hold_chains;
}

int HBChartNative::get_layer_i(String p_layer_name) {
    for (int i = 0; i < layers.size(); i++) {
        Dictionary layer = layers[i];
        if ((String)layer["name"] == p_layer_name) {
            return i;
        }
    }
    return -1;
}
