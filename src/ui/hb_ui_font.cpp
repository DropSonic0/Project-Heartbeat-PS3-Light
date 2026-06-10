#include "hb_ui_font.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBUIFontNative::_bind_methods() {
    BIND_ENUM_CONSTANT(NORMAL);
    BIND_ENUM_CONSTANT(BOLD);
    BIND_ENUM_CONSTANT(BLACK);
}

HBUIFontNative::HBUIFontNative() {}
HBUIFontNative::~HBUIFontNative() {}
