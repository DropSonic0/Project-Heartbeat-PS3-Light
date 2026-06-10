#ifndef HB_GAME_HPP
#define HB_GAME_HPP

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>

namespace godot {

class HBGameNative : public Node {
    GDCLASS(HBGameNative, Node);

protected:
    static void _bind_methods();

private:
    bool demo_mode = false;
    Dictionary serializable_types;

public:
    HBGameNative();
    ~HBGameNative();

    void set_demo_mode(bool p_demo_mode);
    bool get_demo_mode() const;

    Dictionary get_serializable_types() const;
    void register_serializable_type(const String &p_name, const Variant &p_type);

    static HBGameNative *get_singleton();
};

} // namespace godot

#endif // HB_GAME_HPP
