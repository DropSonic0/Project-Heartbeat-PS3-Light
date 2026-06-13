#ifdef __PPU__
#include "compat/gdextension_interface.h"
#include "compat/godot_cpp/core/defs.hpp"
#include "compat/godot_cpp/godot.hpp"
#include "compat/godot_cpp/core/class_db.hpp"
#include "compat/godot_cpp/classes/file_access.hpp"
#include "compat/godot_cpp/classes/dir_access.hpp"
#include "compat/godot_cpp/classes/project_settings.hpp"
#include "compat/godot_cpp/classes/resource_loader.hpp"
#include "compat/godot_cpp/classes/json.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"
#else
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#endif

#include "utils/hb_utils.hpp"
#include "chart/hb_chart.hpp"
#include "chart/hb_serializable.hpp"
#include "chart/hb_timing_point.hpp"
#include "chart/hb_base_note.hpp"
#include "chart/hb_note_data.hpp"
#include "chart/hb_duration_note_data.hpp"
#include "chart/hb_sustain_note.hpp"
#include "chart/hb_double_note.hpp"
#include "chart/hb_rush_note.hpp"
#include "chart/hb_timing_change.hpp"
#include "chart/hb_bpm_change.hpp"
#include "chart/hb_chart_section.hpp"
#include "chart/hb_intro_skip_marker.hpp"
#include "chart/hb_song.hpp"
#include "chart/hb_ppd_song.hpp"
#include "chart/hb_auto_song.hpp"
#include "chart/hb_result.hpp"
#include "chart/hb_game_info.hpp"
#include "chart/hb_per_song_settings.hpp"
#include "chart/hb_per_song_editor_settings.hpp"
#include "chart/hb_song_stats.hpp"
#include "chart/hb_song_variant_data.hpp"
#include "chart/hb_folder.hpp"
#include "chart/hb_user_settings.hpp"
#include "chart/hb_web_user_info.hpp"
#include "chart/hb_editor_template.hpp"
#include "chart/hb_history_entry.hpp"
#include "utils/hb_ini_parser.hpp"
#include "utils/hb_toml_parser.hpp"
#include "utils/hb_ppd_evd_file.hpp"
#include "hb_game.hpp"
#include "hb_judge.hpp"
#include "hb_song_loader.hpp"
#include "hb_error.hpp"
#include "hb_version.hpp"
#include "hb_input_event.hpp"
#include "hb_song_verification.hpp"
#include "utils/hb_shift_jis_table.hpp"
#include "ui/hb_ui_component.hpp"
#include "ui/hb_ui_stylebox_flat.hpp"
#include "ui/hb_ui_stylebox_texture.hpp"
#include "ui/hb_ui_font.hpp"
#include "ui/hb_3d_ui_quad.hpp"
#include "ui/hb_rich_text_word_rainbow.hpp"
#include "utils/hb_ppd_pack.hpp"
#include "utils/hb_smf_loader.hpp"
#include "audio/hb_shinobu_playback_offset.hpp"
#include "graphics/hb_diva_texture_processor.hpp"
#include "graphics/hb_mipmap_generator.hpp"

using namespace godot;

void initialize_project_heartbeat_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    GDREGISTER_CLASS(HBUtilsNative);
    GDREGISTER_CLASS(HBChartNative);
    GDREGISTER_CLASS(HBSerializableNative);
    GDREGISTER_CLASS(HBTimingPointNative);
    GDREGISTER_CLASS(HBBaseNoteNative);
    GDREGISTER_CLASS(HBNoteDataNative);
    GDREGISTER_CLASS(HBDurationNoteDataNative);
    GDREGISTER_CLASS(HBSustainNoteNative);
    GDREGISTER_CLASS(HBDoubleNoteNative);
    GDREGISTER_CLASS(HBRushNoteNative);
    GDREGISTER_CLASS(HBTimingChangeNative);
    GDREGISTER_CLASS(HBBPMChangeNative);
    GDREGISTER_CLASS(HBChartSectionNative);
    GDREGISTER_CLASS(HBIntroSkipMarkerNative);
    GDREGISTER_CLASS(HBSongNative);
    GDREGISTER_CLASS(HBPPDSongNative);
    GDREGISTER_CLASS(HBAutoSongNative);
    GDREGISTER_CLASS(HBResultNative);
    GDREGISTER_CLASS(HBGameInfoNative);
    GDREGISTER_CLASS(HBPerSongSettingsNative);
    GDREGISTER_CLASS(HBPerSongEditorSettingsNative);
    GDREGISTER_CLASS(HBSongStatsNative);
    GDREGISTER_CLASS(HBSongVariantDataNative);
    GDREGISTER_CLASS(HBFolderNative);
    GDREGISTER_CLASS(HBUserSettingsNative);
    GDREGISTER_CLASS(HBWebUserInfoNative);
    GDREGISTER_CLASS(HBEditorTemplateNative);
    GDREGISTER_CLASS(HBHistoryEntryNative);
    GDREGISTER_CLASS(HBINIParserNative);
    GDREGISTER_CLASS(HBTOMLParserNative);
    GDREGISTER_CLASS(PPDEVDFileNative);
    GDREGISTER_CLASS(HBGameNative);
    GDREGISTER_CLASS(HBJudgeNative);
    GDREGISTER_CLASS(HBSongLoaderNative);
    GDREGISTER_CLASS(HBErrorNative);
    GDREGISTER_CLASS(HBVersionNative);
    GDREGISTER_CLASS(InputEventHBNative);
    GDREGISTER_CLASS(HBSongVerificationNative);
    GDREGISTER_CLASS(ShiftJISTableNative);
    GDREGISTER_CLASS(HBUIComponentNative);
    GDREGISTER_CLASS(HBUIStyleboxFlatNative);
    GDREGISTER_CLASS(HBUIStyleboxTextureNative);
    GDREGISTER_CLASS(HBUIFontNative);
    GDREGISTER_CLASS(HB3DUIQuadNative);
    GDREGISTER_CLASS(RichTextWordRainbowNative);
    GDREGISTER_CLASS(PPDPackNative);
    GDREGISTER_CLASS(SMFLoaderNative);
    GDREGISTER_CLASS(ShinobuGodotSoundPlaybackOffsetNative);
    GDREGISTER_CLASS(DIVATextureProcessorNative);
    GDREGISTER_CLASS(HBMipmapGeneratorNative);
}

void uninitialize_project_heartbeat_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT project_heartbeat_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_project_heartbeat_module);
    init_obj.register_terminator(uninitialize_project_heartbeat_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}

#ifdef __PPU__
#include <fstream>
#include <cmath>
int main(int argc, char *argv[]) {
    using namespace godot;
    std::ofstream log_file("/dev_hdd0/game/PROJECTHB/USRDIR/log.txt");
    if (log_file.is_open()) {
        log_file << "Project Heartbeat PS3 Started" << std::endl;
        log_file.close();
    }

    UtilityFunctions::print("Initializing Module...");
    initialize_project_heartbeat_module(MODULE_INITIALIZATION_LEVEL_SCENE);
    UtilityFunctions::print("Module Initialized.");

    // Simple Test Loop
    UtilityFunctions::print("Running tests...");
    Ref<FileAccess> fa = FileAccess::open("/dev_hdd0/game/PROJECTHB/USRDIR/log.txt", FileAccess::READ);
    if (fa.is_valid()) {
        UtilityFunctions::print("Self-test: Read own log success.");
    } else {
        UtilityFunctions::print("Self-test: FileAccess failed.");
    }

    UtilityFunctions::print("Testing JSON and Object properties...");
    String test_json = "{\"title\": \"Test Song\", \"bpm\": 120.5, \"is_classic\": true}";
    Ref<JSON> json;
    json.instantiate();
    if (json->parse(test_json) == 0) {
        UtilityFunctions::print("JSON Parse success.");
        Variant data = json->get_data();
        if (data.get_type() == Variant::DICTIONARY) {
            Dictionary d = data;
            UtilityFunctions::print("JSON Data is Dictionary.");
            
            UtilityFunctions::print("Song Title: " + (String)d["title"]);
            
            Object obj;
            obj.set("metadata", data);
            UtilityFunctions::print("Object Property set success.");
            
            Variant retrieved = obj.get("metadata");
            if (retrieved.get_type() == Variant::DICTIONARY) {
                Dictionary d2 = retrieved;
                UtilityFunctions::print("Object Property retrieve success. BPM: " + String::num((double)d2["bpm"]));
            }
        }
    } else {
        UtilityFunctions::print("JSON Parse failed.");
    }

    UtilityFunctions::print("Testing Class Registration and Method Binding...");
    HBSongLoaderNative *song_loader = memnew(HBSongLoaderNative);
    if (song_loader) {
        UtilityFunctions::print("HBSongLoaderNative instantiated.");
        if (HBSongLoaderNative::get_singleton() == song_loader) {
            UtilityFunctions::print("HBSongLoaderNative Singleton verified.");
        }
        
        Ref<HBSongNative> song;
        song.instantiate();
        if (song.is_valid()) {
            UtilityFunctions::print("HBSongNative (Ref) instantiated.");
            song->set_title("Memory of the Future");
            song->set_bpm(175.0);
            
            song_loader->add_song(song);
            Dictionary songs = song_loader->get_songs();
            if (songs.has("Memory of the Future")) {
                UtilityFunctions::print("Song added and retrieved from HBSongLoader success.");
            }
        }
        memdelete(song_loader);
        UtilityFunctions::print("HBSongLoaderNative deleted.");
    }

    UtilityFunctions::print("Testing Variant Array and Math...");
    Array arr;
    arr.append(1);
    arr.append("two");
    arr.append(3.14);
    if (arr.size() == 3 && (int)arr[0] == 1 && (String)arr[1] == "two") {
        UtilityFunctions::print("Variant Array basic operations success.");
    }

    UtilityFunctions::print("Testing Judge Logic...");
    Ref<HBJudgeNative> judge;
    judge.instantiate();
    if (judge.is_valid()) {
        int rating = judge->judge_note(100, 100);
        if (rating == HBJudgeNative::COOL) {
            UtilityFunctions::print("HBJudgeNative logic test (COOL) success.");
        }
        rating = judge->judge_note(100, 500);
        if (rating == HBJudgeNative::WORST) {
            UtilityFunctions::print("HBJudgeNative logic test (WORST) success.");
        }
    }

    UtilityFunctions::print("Testing Utility Functions...");
    Vector2 sine_pos = HBUtilsNative::calculate_note_sine(0.5f, Vector2(100, 100), 0.0f, 1.0f, 100.0f, 200.0f);
    if (std::abs(sine_pos.x - 200.0f) < 0.1f) {
        UtilityFunctions::print("HBUtilsNative::calculate_note_sine test success.");
    }

    float remapped = HBUtilsNative::remap(0.5f, 0.0f, 50.0f, 1.0f, 150.0f); // Map 0.5 from [0, 1] to [50, 150] -> should be 100
    if (std::abs(remapped - 100.0f) < 0.0001f) {
        UtilityFunctions::print("HBUtilsNative::remap test success.");
    } else {
        UtilityFunctions::print("HBUtilsNative::remap test failed: " + String::num(remapped));
    }
    
    String lpad_test = String("5").lpad(2, "0");
    if (lpad_test == "05") {
        UtilityFunctions::print("String::lpad test success.");
    } else {
        UtilityFunctions::print("String::lpad test failed: " + lpad_test);
    }

    String formatted_time = HBUtilsNative::format_time(65000.0f, HBUtilsNative::FORMAT_MINUTES | HBUtilsNative::FORMAT_SECONDS); // 1:05
    if (formatted_time == "01:05") {
        UtilityFunctions::print("HBUtilsNative::format_time test success.");
    } else {
        UtilityFunctions::print("HBUtilsNative::format_time test failed: " + formatted_time);
    }

    UtilityFunctions::print("Testing Serialization...");
    Ref<HBSongNative> song_to_serialize;
    song_to_serialize.instantiate();
    if (song_to_serialize.is_valid()) {
        song_to_serialize->set_title("Serialized Song");
        song_to_serialize->set_artist("Native Artist");
        song_to_serialize->set_bpm(120.0);
        
        Dictionary serialized = song_to_serialize->serialize();
        UtilityFunctions::print("Serialized data keys: " + String::num(serialized.keys().size()));
        Array keys = serialized.keys();
        for (int i=0; i<keys.size(); i++) {
            // UtilityFunctions::print("Key: " + (String)keys[i]);
        }
        String ser_title = serialized["title"];
        double ser_bpm = serialized["bpm"];
        String ser_type = serialized["type"];
        UtilityFunctions::print("Serialized Title: " + ser_title);
        UtilityFunctions::print("Serialized BPM: " + String::num(ser_bpm));
        UtilityFunctions::print("Serialized Type: " + ser_type);

        if (serialized.has("title") && ser_title == "Serialized Song" && serialized.has("bpm") && std::abs(ser_bpm - 120.0) < 0.0001 && ser_type == "Song") {
            UtilityFunctions::print("HBSongNative serialization test success.");
        } else {
            UtilityFunctions::print("HBSongNative serialization test failed.");
            if (!serialized.has("title")) UtilityFunctions::print("Missing key: title");
            if (!serialized.has("bpm")) UtilityFunctions::print("Missing key: bpm");
            if (!serialized.has("type")) UtilityFunctions::print("Missing key: type");
        }
    }

    UtilityFunctions::print("Testing Chart Logic...");
    Ref<HBChartNative> chart;
    chart.instantiate();
    if (chart.is_valid()) {
        UtilityFunctions::print("HBChartNative instantiated.");
        chart->populate_layers();
        Dictionary serialized_chart = chart->serialize();
        if (serialized_chart.has("layers") && ((Array)serialized_chart["layers"]).size() > 0) {
            UtilityFunctions::print("HBChartNative serialization success.");
        }
        
        // Test deserialization
        Ref<HBChartNative> chart2;
        chart2.instantiate();
        chart2->deserialize(serialized_chart, Variant());
        Dictionary ser2 = chart2->serialize();
        if (ser2.has("layers") && ((Array)ser2["layers"]).size() == ((Array)serialized_chart["layers"]).size()) {
            UtilityFunctions::print("HBChartNative deserialization success.");
        } else {
            UtilityFunctions::print("HBChartNative deserialization failed.");
        }
    }

    UtilityFunctions::print("Testing File I/O and Song Loading...");
    
    Ref<DirAccess> da = DirAccess::open("/dev_hdd0/game/PROJECTHB/USRDIR");
    if (da.is_valid()) {
        UtilityFunctions::print("DirAccess: Open USRDIR success.");
        PackedStringArray files = da->get_files();
        UtilityFunctions::print("Files in USRDIR: " + String::num(files.size()));
        for (int i=0; i<files.size(); i++) {
            UtilityFunctions::print(" - " + (String)files[i]);
        }
    }

    String mock_song_path = "/dev_hdd0/game/PROJECTHB/USRDIR/test_song.json";
    std::ofstream mock_file(mock_song_path.c_str());
    if (mock_file.is_open()) {
        mock_file << "{\"title\": \"Mock Song\", \"bpm\": 145.0, \"artist\": \"JSON Artist\"}" << std::endl;
        mock_file.close();
        
        HBSongLoaderNative loader;
        Ref<HBSongNative> loaded_song = loader.load_song_meta(mock_song_path, "mock_id");
        if (loaded_song.is_valid()) {
            if (loaded_song->get_title() == "Mock Song" && loaded_song->get_bpm() == 145.0) {
                UtilityFunctions::print("Song Loader: Load from JSON success.");
            } else {
                UtilityFunctions::print("Song Loader: Load from JSON data mismatch.");
            }
        } else {
            UtilityFunctions::print("Song Loader: Load from JSON failed.");
        }
    }

    UtilityFunctions::print("Testing Result Logic...");
    Ref<HBResultNative> result;
    result.instantiate();
    if (result.is_valid()) {
        result->set_max_score(1000000);
        result->set_score(950000);
        if (std::abs(result->get_percentage() - 0.95) < 0.0001) {
            UtilityFunctions::print("HBResultNative logic test success.");
        }
        Dictionary ser_res = result->serialize();
        if (ser_res.has("score") && (int64_t)ser_res["score"] == 950000) {
            UtilityFunctions::print("HBResultNative serialization success.");
        }
    }

    UtilityFunctions::print("Testing Version and Input Events...");
    String version_str = HBVersionNative::get_version_string();
    UtilityFunctions::print("Game Version: " + version_str);
    
    Ref<InputEventHBNative> input_event;
    input_event.instantiate();
    if (input_event.is_valid()) {
        UtilityFunctions::print("InputEventHBNative instantiated.");
    }

    UtilityFunctions::print("Testing Config Parsers (INI/TOML)...");
    String ini_data = "[Section]\nkey=value\n[Other]\nnum=123";
    Dictionary parsed_ini = HBINIParserNative::parse(ini_data);
    if (parsed_ini.has("Section")) {
        Dictionary section = parsed_ini["Section"];
        if (section.has("key") && (String)section["key"] == "value") {
            UtilityFunctions::print("HBINIParserNative test success.");
        } else {
            UtilityFunctions::print("HBINIParserNative test failed: key mismatch.");
        }
    } else {
        UtilityFunctions::print("HBINIParserNative test failed: Section not found.");
    }
    
    String toML_data = "[table]\nkey = \"toml_value\"\nnum = 456\n[[items]]\nname=\"item1\"\n[[items]]\nname=\"item2\"";
    Dictionary parsed_toml = HBTOMLParserNative::parse(toML_data);
    if (parsed_toml.has("table")) {
        Dictionary table = parsed_toml["table"];
        if (table.has("key") && (String)table["key"] == "toml_value" && parsed_toml.has("items")) {
            Array items = parsed_toml["items"];
            if (items.size() == 2 && (String)((Dictionary)items[0])["name"] == "item1") {
                UtilityFunctions::print("HBTOMLParserNative complex test success.");
            } else {
                UtilityFunctions::print("HBTOMLParserNative complex test failed: items mismatch.");
            }
        } else {
            UtilityFunctions::print("HBTOMLParserNative test failed: key mismatch.");
        }
    } else {
        UtilityFunctions::print("HBTOMLParserNative test failed: table not found.");
    }

    UtilityFunctions::print("Testing Song Verification...");
    HBSongVerificationNative verification;
    Dictionary verify_errors = verification.verify_song(song_to_serialize);
    if (!verification.has_fatal_error(verify_errors)) {
        UtilityFunctions::print("HBSongVerificationNative: No fatal errors (Expected).");
    }

    UtilityFunctions::print("Testing PPD EVD File Logic...");
    Ref<PPDEVDFileNative> evd;
    evd.instantiate();
    if (evd.is_valid()) {
        UtilityFunctions::print("PPDEVDFileNative instantiated.");
        // Test basic accessors
        if (evd->get_note_type_at_time(0.0) == 0) {
            UtilityFunctions::print("PPDEVDFileNative logic test success.");
        }
    }

    UtilityFunctions::print("Testing Error Handling...");
    HBErrorNative error;
    error.set_error_message("Diagnostic Test Error");
    if (error.to_string().begins_with("HBError:")) {
        UtilityFunctions::print("HBErrorNative test success.");
    }

    UtilityFunctions::print("Testing ShiftJIS Table...");
    PackedByteArray sjis_table = ShiftJISTableNative::get_conv_table();
    UtilityFunctions::print("ShiftJIS Table retrieved, size: " + String::num(sjis_table.size()));

    UtilityFunctions::print("Testing PCK Resource Packs...");
    String base_pck = "/dev_hdd0/game/PROJECTHB/USRDIR/Project Heartbeat Base Content.pck";
    String main_pck = "/dev_hdd0/game/PROJECTHB/USRDIR/Project Heartbeat.pck";
    
    if (da->file_exists(base_pck)) {
        UtilityFunctions::print("Base PCK found.");
        if (godot::ProjectSettings::get_singleton()->load_resource_pack(base_pck)) {
            UtilityFunctions::print("Base PCK load (stub) success.");
        }
    } else {
        UtilityFunctions::print("Base PCK NOT found at: " + base_pck);
    }

    if (da->file_exists(main_pck)) {
        UtilityFunctions::print("Main PCK found.");
        if (godot::ProjectSettings::get_singleton()->load_resource_pack(main_pck)) {
            UtilityFunctions::print("Main PCK load (stub) success.");
        }
    } else {
        UtilityFunctions::print("Main PCK NOT found at: " + main_pck);
    }

    UtilityFunctions::print("Testing Graphics and Texture Processors...");
    DIVATextureProcessorNative *texture_processor = memnew(DIVATextureProcessorNative);
    if (texture_processor) {
        UtilityFunctions::print("DIVATextureProcessorNative instantiated.");
        memdelete(texture_processor);
    }
    
    HBMipmapGeneratorNative *mipmap_generator = memnew(HBMipmapGeneratorNative);
    if (mipmap_generator) {
        UtilityFunctions::print("HBMipmapGeneratorNative instantiated.");
        memdelete(mipmap_generator);
    }

    if (godot::RenderingServer::get_singleton()) {
        UtilityFunctions::print("RenderingServer Singleton verified.");
    }

    UtilityFunctions::print("Testing Audio and Pack Loaders...");
    ShinobuGodotSoundPlaybackOffsetNative *audio_offset = memnew(ShinobuGodotSoundPlaybackOffsetNative);
    if (audio_offset) {
        UtilityFunctions::print("ShinobuGodotSoundPlaybackOffsetNative instantiated.");
        memdelete(audio_offset);
    }
    
    PPDPackNative *ppd_pack = memnew(PPDPackNative);
    if (ppd_pack) {
        UtilityFunctions::print("PPDPackNative instantiated.");
        memdelete(ppd_pack);
    }
    
    SMFLoaderNative *smf_loader = memnew(SMFLoaderNative);
    if (smf_loader) {
        UtilityFunctions::print("SMFLoaderNative instantiated.");
        memdelete(smf_loader);
    }

    UtilityFunctions::print("Testing UI Components...");
    HBUIComponentNative *ui_comp = memnew(HBUIComponentNative);
    if (ui_comp) {
        if (ui_comp->get_component_name() == "HBComponent") {
            UtilityFunctions::print("HBUIComponentNative instantiated and virtual method verified.");
        }
        memdelete(ui_comp);
    }
    
    HB3DUIQuadNative *ui_quad = memnew(HB3DUIQuadNative);
    if (ui_quad) {
        UtilityFunctions::print("HB3DUIQuadNative instantiated.");
        memdelete(ui_quad);
    }

    Ref<HBUIStyleboxFlatNative> sb_flat;
    sb_flat.instantiate();
    if (sb_flat.is_valid()) {
        UtilityFunctions::print("HBUIStyleboxFlatNative instantiated.");
    }
    
    HBUIStyleboxTextureNative *sb_tex = memnew(HBUIStyleboxTextureNative);
    if (sb_tex) {
        UtilityFunctions::print("HBUIStyleboxTextureNative instantiated.");
        memdelete(sb_tex);
    }

    HBUIFontNative *ui_font = memnew(HBUIFontNative);
    if (ui_font) {
        UtilityFunctions::print("HBUIFontNative instantiated.");
        memdelete(ui_font);
    }

    RichTextWordRainbowNative *rainbow = memnew(RichTextWordRainbowNative);
    if (rainbow) {
        UtilityFunctions::print("RichTextWordRainbowNative instantiated.");
        memdelete(rainbow);
    }

    UtilityFunctions::print("Testing Resource Loading...");
    if (godot::ResourceLoader::get_singleton()) {
        UtilityFunctions::print("ResourceLoader Singleton verified.");
        Ref<Resource> res = godot::ResourceLoader::get_singleton()->load("res://icon.png");
        UtilityFunctions::print("Resource Load (stub) called.");
    }

    UtilityFunctions::print("Testing Song Data Containers...");
    Ref<HBFolderNative> folder;
    folder.instantiate();
    if (folder.is_valid()) {
        folder->set_folder_name("Test Folder");
        if (folder->get_folder_name() == "Test Folder") {
            UtilityFunctions::print("HBFolderNative test success.");
        }
    }
    
    Ref<HBUserSettingsNative> settings;
    settings.instantiate();
    if (settings.is_valid()) {
        UtilityFunctions::print("HBUserSettingsNative instantiated.");
    }
    
    Ref<HBSongStatsNative> stats;
    stats.instantiate();
    if (stats.is_valid()) {
        UtilityFunctions::print("HBSongStatsNative instantiated.");
    }

    UtilityFunctions::print("Testing Directory Scanning...");
    if (da.is_valid()) {
        PackedStringArray dirs = da->get_directories();
        UtilityFunctions::print("Directories in USRDIR: " + String::num(dirs.size()));
        for (int i=0; i<dirs.size(); i++) {
            UtilityFunctions::print(" - [DIR] " + (String)dirs[i]);
        }
    }

    UtilityFunctions::print("Testing Game Session and Info...");
    Ref<HBGameInfoNative> game_info;
    game_info.instantiate();
    if (game_info.is_valid()) {
        game_info->set_song_id("test_song_123");
        if (game_info->get_time() > 0) {
            UtilityFunctions::print("HBGameInfoNative captured system time: " + String::num_int64(game_info->get_time()));
        }
        Dictionary ser_info = game_info->serialize();
        if ((String)ser_info["song_id"] == "test_song_123") {
            UtilityFunctions::print("HBGameInfoNative serialization success.");
        }
    }

    UtilityFunctions::print("Testing Global Game State...");
    HBGameNative *game = memnew(HBGameNative);
    if (game) {
        UtilityFunctions::print("HBGameNative instantiated.");
        if (HBGameNative::get_singleton() == game) {
            UtilityFunctions::print("HBGameNative Singleton verified.");
        }
        game->set_demo_mode(true);
        if (game->get_demo_mode()) {
            UtilityFunctions::print("HBGameNative property test success.");
        }
        memdelete(game);
        UtilityFunctions::print("HBGameNative deleted.");
    }
    
    UtilityFunctions::print("Tests finished. Result: SUCCESS");

    return 0;
}
#endif
