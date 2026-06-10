#ifndef HB_SERIALIZABLE_HPP
#define HB_SERIALIZABLE_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace godot {

class HBSerializableNative : public RefCounted {
    GDCLASS(HBSerializableNative, RefCounted);

protected:
    static void _bind_methods();
    Array serializable_fields;

public:
    HBSerializableNative();
    ~HBSerializableNative();

    virtual Dictionary serialize(bool p_serialize_defaults = false);
    virtual String get_serialized_type() const;
};

} // namespace godot

#endif // HB_SERIALIZABLE_HPP
