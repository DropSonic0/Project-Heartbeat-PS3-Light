#ifndef HB_MIPMAP_GENERATOR_HPP
#define HB_MIPMAP_GENERATOR_HPP

#include <godot_cpp/classes/object.hpp>

namespace godot {

class HBMipmapGeneratorNative : public Object {
    GDCLASS(HBMipmapGeneratorNative, Object);

protected:
    static void _bind_methods();

public:
    HBMipmapGeneratorNative();
    ~HBMipmapGeneratorNative();
};

} // namespace godot

#endif // HB_MIPMAP_GENERATOR_HPP
