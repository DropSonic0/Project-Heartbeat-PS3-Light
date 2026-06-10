#ifndef HB_SMF_LOADER_HPP
#define HB_SMF_LOADER_HPP

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

namespace godot {

class SMFLoaderNative : public Object {
    GDCLASS(SMFLoaderNative, Object);

protected:
    static void _bind_methods();

public:
    SMFLoaderNative();
    ~SMFLoaderNative();
};

} // namespace godot

#endif // HB_SMF_LOADER_HPP
