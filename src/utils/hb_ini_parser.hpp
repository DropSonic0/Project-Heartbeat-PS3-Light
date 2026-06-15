#ifndef HB_INI_PARSER_HPP
#define HB_INI_PARSER_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/object.hpp"
#else
#include <godot_cpp/classes/object.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/variant/dictionary.hpp"
#else
#include <godot_cpp/variant/dictionary.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/variant/string.hpp"
#else
#include <godot_cpp/variant/string.hpp>
#endif

namespace godot {

class HBINIParserNative : public Object {
    GDCLASS(HBINIParserNative, Object);

protected:
    static void _bind_methods();

public:
    static Dictionary parse(const String &p_contents);
};

} // namespace godot

#endif // HB_INI_PARSER_HPP
