#include "hb_3d_ui_quad.hpp"
#ifdef __PPU__
#include "compat/godot_cpp/core/class_db.hpp"
#else
#include <godot_cpp/core/class_db.hpp>
#endif

using namespace godot;

void HB3DUIQuadNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_size", "size"), &HB3DUIQuadNative::set_size);
    ClassDB::bind_method(D_METHOD("get_size"), &HB3DUIQuadNative::get_size);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "size"), "set_size", "get_size");
}

HB3DUIQuadNative::HB3DUIQuadNative() {
    node_viewport = nullptr;
    node_quad = nullptr;
    node_area = nullptr;
}
HB3DUIQuadNative::~HB3DUIQuadNative() {}

void HB3DUIQuadNative::_ready() {
    node_viewport = memnew(SubViewport);
    add_child(node_viewport);

    node_quad = memnew(MeshInstance3D);
    add_child(node_quad);

    node_area = memnew(Area3D);
    add_child(node_area);
}
void HB3DUIQuadNative::_process(double p_delta) {}

void HB3DUIQuadNative::set_size(Vector2 p_size) {
    size = p_size;
}

Vector2 HB3DUIQuadNative::get_size() const {
    return size;
}
