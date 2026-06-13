#include "hb_serializable.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBSerializableNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("serialize", "serialize_defaults"), &HBSerializableNative::serialize, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("get_serialized_type"), &HBSerializableNative::get_serialized_type);
}

HBSerializableNative::HBSerializableNative() {}
HBSerializableNative::~HBSerializableNative() {}

Dictionary HBSerializableNative::serialize(bool p_serialize_defaults) {
    Dictionary data;
    for (int i = 0; i < serializable_fields.size(); i++) {
        String field = serializable_fields[i];
        Variant val = get(field);
        if (val.get_type() == Variant::OBJECT) {
            Object *obj = (Object*)val;
            if (obj && obj->has_method("serialize")) {
                data[field] = obj->call("serialize", p_serialize_defaults, Variant());
            } else {
                data[field] = val;
            }
        } else {
            data[field] = val;
        }
    }
    data["type"] = get_serialized_type();
    return data;
}

String HBSerializableNative::get_serialized_type() const {
    return "Serializable";
}

bool HBSerializableNative::has_method(const std::string &p_method) const {
    if (p_method == "serialize") return true;
    return Object::has_method(p_method);
}

Variant HBSerializableNative::call(const std::string &p_method, const Variant& p_arg1, const Variant& p_arg2) {
    if (p_method == "serialize") {
        return serialize((bool)p_arg1);
    }
    return Object::call(p_method, p_arg1, p_arg2);
}
