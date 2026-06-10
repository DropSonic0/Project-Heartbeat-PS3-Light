#ifndef HB_TOML_PARSER_HPP
#define HB_TOML_PARSER_HPP

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

class HBTOMLParserNative : public Object {
    GDCLASS(HBTOMLParserNative, Object);

protected:
    static void _bind_methods();

public:
    static Dictionary parse(const String &p_contents);
    static Array parse_key(const String &p_key);
};

} // namespace godot

#endif // HB_TOML_PARSER_HPP
