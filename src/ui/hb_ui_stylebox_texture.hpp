#ifndef HB_UI_STYLEBOX_TEXTURE_HPP
#define HB_UI_STYLEBOX_TEXTURE_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/style_box_texture.hpp"
#else
#include <godot_cpp/classes/style_box_texture.hpp>
#endif

namespace godot {

class HBUIStyleboxTextureNative : public StyleBoxTexture {
    GDCLASS(HBUIStyleboxTextureNative, StyleBoxTexture);

protected:
    static void _bind_methods();

public:
    HBUIStyleboxTextureNative();
    ~HBUIStyleboxTextureNative();
};

} // namespace godot

#endif // HB_UI_STYLEBOX_TEXTURE_HPP
