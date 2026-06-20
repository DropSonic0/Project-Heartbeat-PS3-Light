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
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/json.hpp>
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
#include "graphics/hb_video_driver_psgl.hpp"

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

    UtilityFunctions::print("Starting Game Session...");
    HBGameNative *game = memnew(HBGameNative);
    if (game) {
        UtilityFunctions::print("HBGameNative instantiated for main loop.");
        
        UtilityFunctions::print("Entering main loop...");
        // In a real Godot app, the engine handles the loop.
        // Here we manually step the game native class.
        int frame_count = 0;
        while (!godot::HBVideoDriverPSGL::should_exit()) {
            game->main_loop_step();
            frame_count++;
            if (frame_count % 600 == 0) {
                UtilityFunctions::print("Main loop running... frame: " + String::num(frame_count));
            }
        }
        UtilityFunctions::print("Exiting main loop... (Exit requested by system)");
        memdelete(game);
    } else {
        UtilityFunctions::print("FATAL: Failed to instantiate HBGameNative for main loop.");
    }

    UtilityFunctions::print("Project Heartbeat PS3 Shutting Down.");
    return 0;
}
#endif
