#ifndef HB_DIVA_TEXTURE_PROCESSOR_HPP
#define HB_DIVA_TEXTURE_PROCESSOR_HPP

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

namespace godot {

class DIVATextureProcessorNative : public Object {
    GDCLASS(DIVATextureProcessorNative, Object);

protected:
    static void _bind_methods();

public:
    DIVATextureProcessorNative();
    ~DIVATextureProcessorNative();
};

} // namespace godot

#endif // HB_DIVA_TEXTURE_PROCESSOR_HPP
