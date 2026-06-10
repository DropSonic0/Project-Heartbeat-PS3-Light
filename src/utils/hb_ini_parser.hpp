#ifndef HB_INI_PARSER_HPP
#define HB_INI_PARSER_HPP

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/string.hpp>

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
