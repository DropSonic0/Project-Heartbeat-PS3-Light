#ifndef HB_3D_UI_QUAD_HPP
#define HB_3D_UI_QUAD_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/area3d.hpp"
#include "compat/godot_cpp/classes/sub_viewport.hpp"
#include "compat/godot_cpp/classes/mesh_instance3d.hpp"
#else
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#endif

namespace godot {

class HB3DUIQuadNative : public Area3D {
    GDCLASS(HB3DUIQuadNative, Area3D);

protected:
    static void _bind_methods();

private:
    SubViewport *node_viewport;
    MeshInstance3D *node_quad;
    Area3D *node_area;

public:
    HB3DUIQuadNative();
    ~HB3DUIQuadNative();

    void _ready() GD_OVERRIDE;
    void _process(double p_delta) GD_OVERRIDE;
};

} // namespace godot

#endif // HB_3D_UI_QUAD_HPP
