#ifndef HB_SMF_LOADER_HPP
#define HB_SMF_LOADER_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/object.hpp"
#else
#include <godot_cpp/classes/object.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/variant/packed_byte_array.hpp"
#else
#include <godot_cpp/variant/packed_byte_array.hpp>
#endif

namespace godot {

class SMFLoaderNative : public Object {
    GDCLASS(SMFLoaderNative, Object);

protected:
    static void _bind_methods();

public:
    SMFLoaderNative();
    ~SMFLoaderNative();

    Dictionary read_file(const String &p_path);
};

} // namespace godot

#endif // HB_SMF_LOADER_HPP
