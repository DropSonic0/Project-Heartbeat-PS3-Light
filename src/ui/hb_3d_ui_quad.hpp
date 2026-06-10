#ifndef HB_3D_UI_QUAD_HPP
#define HB_3D_UI_QUAD_HPP

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

namespace godot {

class HB3DUIQuadNative : public Area3D {
    GDCLASS(HB3DUIQuadNative, Area3D);

protected:
    static void _bind_methods();

private:
    SubViewport *node_viewport = nullptr;
    MeshInstance3D *node_quad = nullptr;
    Area3D *node_area = nullptr;

public:
    HB3DUIQuadNative();
    ~HB3DUIQuadNative();

    void _ready() override;
    void _process(double p_delta) override;
};

} // namespace godot

#endif // HB_3D_UI_QUAD_HPP
