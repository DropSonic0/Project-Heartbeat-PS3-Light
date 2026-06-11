#include "hb_3d_ui_quad.hpp"
#ifdef __PPU__
#include "compat/godot_cpp/core/class_db.hpp"
#else
#include <godot_cpp/core/class_db.hpp>
#endif

using namespace godot;

void HB3DUIQuadNative::_bind_methods() {}

HB3DUIQuadNative::HB3DUIQuadNative() {
    node_viewport = nullptr;
    node_quad = nullptr;
    node_area = nullptr;
}
HB3DUIQuadNative::~HB3DUIQuadNative() {}

void HB3DUIQuadNative::_ready() {}
void HB3DUIQuadNative::_process(double p_delta) {}
