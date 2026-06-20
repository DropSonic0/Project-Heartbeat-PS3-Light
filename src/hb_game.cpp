#include "hb_game.hpp"
#include <godot_cpp/core/class_db.hpp>
#include "graphics/hb_video_driver_psgl.hpp"
#include "ui/hb_main_menu_native.hpp"
#include "ui/hb_song_list_native.hpp"
#include "ui/hb_options_menu_native.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"
#include "compat/godot_cpp/classes/file_access.hpp"
#include "compat/godot_cpp/classes/project_settings.hpp"
#include "compat/godot_cpp/classes/dir_access.hpp"
#include "compat/godot_cpp/classes/image.hpp"
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

    // Load splash image from disk
    String splash_path = "/dev_hdd0/game/PROJECTHB/USRDIR/Present.jpg";
    Ref<FileAccess> fa = FileAccess::open(splash_path, FileAccess::READ);
    if (fa.is_valid()) {
        PackedByteArray buffer = fa->get_buffer((size_t)fa->get_length());
        splash_image = Image::load_from_buffer(buffer);
        if (splash_image.is_valid()) {
            UtilityFunctions::print("Loaded initial splash image: " + splash_path);
        } else {
            UtilityFunctions::print("Failed to decode initial splash image: " + splash_path);
        }
    } else {
        UtilityFunctions::print("Initial splash image not found: " + splash_path);
    }

    is_loading = true;
    loading_thread = memnew(Threads::Thread(&HBGameNative::_load_resources, this));
}

HBGameNative::~HBGameNative() {
    if (loading_thread) {
        loading_thread->join();
        memdelete(loading_thread);
    }
    if (current_ui) {
        memdelete(current_ui);
    }
    if (song_loader) {
        memdelete(song_loader);
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

void HBGameNative::change_to_menu(const String& p_menu, const String& p_param) {
    if (current_ui) {
        memdelete(current_ui);
        current_ui = nullptr;
    }

    if (p_menu == "main_menu") {
        HBMainMenuNative::State initial_state = HBMainMenuNative::PRESS_START;
        if (p_param == "skip_splash") {
            initial_state = HBMainMenuNative::MAIN_MENU;
        }
        current_ui = memnew(HBMainMenuNative(initial_state));
    } else if (p_menu == "song_list") {
        current_ui = memnew(HBSongListNative());
    } else if (p_menu == "options_menu") {
        current_ui = memnew(HBOptionsMenuNative());
    }
}

void HBGameNative::_load_resources() {
    UtilityFunctions::print("HBGameNative: Background loading started.");

    Ref<DirAccess> da = DirAccess::open("/dev_hdd0/game/PROJECTHB/USRDIR");
    if (da.is_valid()) {
        String base_pck = "/dev_hdd0/game/PROJECTHB/USRDIR/Project Heartbeat Base Content.pck";
        String main_pck = "/dev_hdd0/game/PROJECTHB/USRDIR/Project Heartbeat.pck";

        if (da->file_exists(base_pck)) {
            godot::ProjectSettings::get_singleton()->load_resource_pack(base_pck);
        }
        if (da->file_exists(main_pck)) {
            godot::ProjectSettings::get_singleton()->load_resource_pack(main_pck);
        }
    }

    song_loader = memnew(HBSongLoaderNative());
    song_loader->scan_songs_recursive("/dev_hdd0/game/PROJECTHB/USRDIR/songs");
    song_loader->scan_songs_recursive("res://songs");

    UtilityFunctions::print("HBGameNative: Background loading finished.");
    {
        Threads::ScopedLock lock(loading_mutex);
        is_loading = false;
    }
}

void HBGameNative::main_loop_step() {
    HBVideoDriverPSGL::update_system_callbacks();
    
    bool loading = false;
    {
        Threads::ScopedLock lock(loading_mutex);
        loading = is_loading;
    }

    if (loading) {
        HBVideoDriverPSGL::clear_buffer();
        if (splash_image.is_valid()) {
            Vector2 window_size = HBVideoDriverPSGL::get_window_size();
            HBVideoDriverPSGL::draw_texture(splash_image, Rect2(0, 0, window_size.x, window_size.y));
        }
        HBVideoDriverPSGL::swap_buffers();
        return;
    }

    if (!current_ui) {
        current_ui = memnew(HBMainMenuNative());
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

HBGameNative *HBGameNative::get_singleton() {
    return singleton;
}
