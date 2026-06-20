#include "hb_game.hpp"
#include <godot_cpp/core/class_db.hpp>
#include "graphics/hb_video_driver_psgl.hpp"
#include "ui/hb_main_menu_native.hpp"
#include "ui/hb_song_list_native.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"

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
    current_ui = new HBMainMenuNative();
}

HBGameNative::~HBGameNative() {
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

void HBGameNative::change_to_menu(const String& p_menu) {
    if (current_ui) {
        delete current_ui;
        current_ui = nullptr;
    }

    if (p_menu == "main_menu") {
        current_ui = new HBMainMenuNative();
    } else if (p_menu == "song_list") {
        current_ui = new HBSongListNative();
    }
}

void HBGameNative::main_loop_step() {
    HBVideoDriverPSGL::update_system_callbacks();
    
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
