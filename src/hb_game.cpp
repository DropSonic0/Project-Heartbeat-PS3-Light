#include "hb_game.hpp"
#include <godot_cpp/core/class_db.hpp>
#include "graphics/hb_video_driver_psgl.hpp"
#include "ui/hb_main_menu_native.hpp"
#include "ui/hb_song_list_native.hpp"
#include "ui/hb_rhythm_game_native.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"
#include "compat/godot_cpp/classes/file_access.hpp"
#include "compat/godot_cpp/classes/project_settings.hpp"
#include "hb_song_loader.hpp"
#include "threads/scoped_lock.hpp"

using namespace godot;

static HBGameNative *singleton = nullptr;

void HBGameNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_demo_mode", "demo_mode"), &HBGameNative::set_demo_mode);
    ClassDB::bind_method(D_METHOD("get_demo_mode"), &HBGameNative::get_demo_mode);
    ClassDB::bind_method(D_METHOD("get_serializable_types"), &HBGameNative::get_serializable_types);
    ClassDB::bind_method(D_METHOD("register_serializable_type", "name", "type"), &HBGameNative::register_serializable_type);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "demo_mode"), "set_demo_mode", "get_demo_mode");
}

HBGameNative::HBGameNative() {
    singleton = this;
    UtilityFunctions::randomize();
    HBVideoDriverPSGL::initialize();
    
    // Load splash screen
    Ref<FileAccess> fa = FileAccess::open("/dev_hdd0/game/PROJECTHB/USRDIR/Present.jpg", FileAccess::READ);
    if (fa.is_valid()) {
        splash_image = Image::load_from_buffer(fa->get_buffer((size_t)fa->get_length()));
    }
    
    // Ensure song loader exists before background scanning
    if (!HBSongLoaderNative::get_singleton()) {
        memnew(HBSongLoaderNative);
    }

    loading_thread = new Threads::Thread(&HBGameNative::_background_load, this);
}

HBGameNative::~HBGameNative() {
    if (loading_thread) {
        loading_thread->join();
        delete loading_thread;
    }
    if (current_ui) {
        delete current_ui;
    }
    HBVideoDriverPSGL::terminate();
    singleton = nullptr;
}

void HBGameNative::set_demo_mode(bool p_demo_mode) { demo_mode = p_demo_mode; }
bool HBGameNative::get_demo_mode() const { return demo_mode; }

Dictionary HBGameNative::get_serializable_types() const { return serializable_types; }
void HBGameNative::register_serializable_type(const String &p_name, const Variant &p_type) {
    serializable_types[p_name] = p_type;
}

void HBGameNative::change_to_menu(const String& p_menu, const Variant& p_param) {
    if (current_ui) {
        delete current_ui;
        current_ui = nullptr;
    }

    if (p_menu == "main_menu") {
        current_ui = new HBMainMenuNative();
    } else if (p_menu == "song_list") {
        current_ui = new HBSongListNative();
    } else if (p_menu == "rhythm_game") {
        current_ui = new HBRhythmGameNative(p_param);
    }
}

void HBGameNative::main_loop_step() {
    HBVideoDriverPSGL::update_system_callbacks();
    
    bool loading;
    {
        Threads::ScopedLock lock(loading_mutex);
        loading = is_loading;
    }

    if (loading) {
        HBVideoDriverPSGL::clear_buffer();
        if (splash_image.is_valid()) {
            Vector2 screen_size = HBVideoDriverPSGL::get_window_size();
            // Draw splash image centered or scaled? 
            // Original requested "mostrar Present.jpg"
            // Let's draw it covering the screen or centered.
            HBVideoDriverPSGL::draw_texture(splash_image, Rect2(Vector2(0, 0), screen_size));
        }
        HBVideoDriverPSGL::swap_buffers();
    } else {
        if (!current_ui) {
            current_ui = new HBMainMenuNative();
        }
        
        if (current_ui) {
            current_ui->update();
        }

        HBVideoDriverPSGL::clear_buffer();
        
        if (current_ui) {
            current_ui->draw();
        }
        
        HBVideoDriverPSGL::swap_buffers();
    }
}

void HBGameNative::_background_load() {
    UtilityFunctions::print("Background Load: Starting...");
    
    // Load PCK files
    String base_pck = "/dev_hdd0/game/PROJECTHB/USRDIR/Project Heartbeat Base Content.pck";
    String main_pck = "/dev_hdd0/game/PROJECTHB/USRDIR/Project Heartbeat.pck";
    
    if (FileAccess::file_exists(base_pck)) {
        ProjectSettings::get_singleton()->load_resource_pack(base_pck);
    }
    if (FileAccess::file_exists(main_pck)) {
        ProjectSettings::get_singleton()->load_resource_pack(main_pck);
    }

    // Scan songs
    HBSongLoaderNative *song_loader = HBSongLoaderNative::get_singleton();
    if (song_loader) {
        song_loader->scan_songs_recursive("/dev_hdd0/game/PROJECTHB/USRDIR/songs");
        song_loader->scan_songs_recursive("res://songs");
    }

    UtilityFunctions::print("Background Load: Finished.");
    
    {
        Threads::ScopedLock lock(loading_mutex);
        is_loading = false;
    }
}

HBGameNative *HBGameNative::get_singleton() {
    return singleton;
}
