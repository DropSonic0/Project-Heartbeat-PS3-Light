#ifndef HB_DIVA_TEXTURE_PROCESSOR_HPP
#define HB_DIVA_TEXTURE_PROCESSOR_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/object.hpp"
#else
#include <godot_cpp/classes/object.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/classes/rendering_server.hpp"
#else
#include <godot_cpp/classes/rendering_server.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/classes/image.hpp"
#else
#include <godot_cpp/classes/image.hpp>
#endif

namespace godot {

class DIVATextureProcessorNative : public Object {
    GDCLASS(DIVATextureProcessorNative, Object);

protected:
    static void _bind_methods();

public:
    DIVATextureProcessorNative();
    ~DIVATextureProcessorNative();

    void process_texture(Ref<Image> p_image);
};

} // namespace godot

#endif // HB_DIVA_TEXTURE_PROCESSOR_HPP
