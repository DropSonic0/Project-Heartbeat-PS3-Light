#ifndef HB_VERSION_HPP
#define HB_VERSION_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/node.hpp"
#else
#include <godot_cpp/classes/node.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/variant/string.hpp"
#else
#include <godot_cpp/variant/string.hpp>
#endif

namespace godot {

class HBVersionNative : public Node {
    GDCLASS(HBVersionNative, Node);

protected:
    static void _bind_methods();

public:
    static const int MAJOR = 1;
    static const int MINOR = 0;
    static const int PATCH = 0;

    static String get_version_string(bool p_with_line_breaks = false);
};

} // namespace godot

#endif // HB_VERSION_HPP
