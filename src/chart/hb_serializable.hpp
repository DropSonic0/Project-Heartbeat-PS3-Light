#ifndef HB_SERIALIZABLE_HPP
#define HB_SERIALIZABLE_HPP

#include <godot_cpp/godot.hpp>

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

    virtual bool has_method(const std::string &p_method) const override;
    virtual Variant call(const std::string &p_method, const Variant& p_arg1, const Variant& p_arg2) override;
};

} // namespace godot

#endif // HB_SERIALIZABLE_HPP
