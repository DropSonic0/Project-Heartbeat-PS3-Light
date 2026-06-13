#include "classes/object.hpp"
#include "variant/variant.hpp"
#include "variant/utility_functions.hpp"
#include <map>
#include <string>

namespace godot {

static std::map<const Object*, std::map<std::string, Variant> > _object_properties;

void Object::set(const std::string &p_name, const Variant& p_value) {
    _object_properties[this][p_name] = p_value;
}

Variant Object::get(const std::string &p_name) const {
    std::map<const Object*, std::map<std::string, Variant> >::iterator it = _object_properties.find(this);
    if (it != _object_properties.end()) {
        std::map<std::string, Variant>::iterator it2 = it->second.find(p_name);
        if (it2 != it->second.end()) {
            return it2->second;
        }
    }
    return Variant();
}

Object::~Object() {
    _object_properties.erase(this);
}

Variant::~Variant() {
    if (type == ARRAY && array_val) delete array_val;
    if (type == DICTIONARY && dict_val) delete dict_val;
}

bool Object::has_method(const std::string &p_method) const {
    return false;
}

Variant Object::call(const std::string &p_method, const Variant& p_arg1, const Variant& p_arg2) {
    UtilityFunctions::print("Object::call: " + p_method);
    return Variant();
}

Variant Variant::get(const String& p_name) const {
    if (type == OBJECT && obj_val) {
        return obj_val->get(p_name);
    }
    return Variant();
}

Variant Variant::call(const String& p_method, const Variant& p_arg1, const Variant& p_arg2) {
    if (type == OBJECT && obj_val) {
        return obj_val->call(p_method, p_arg1, p_arg2);
    }
    return Variant();
}

Variant& Variant::operator=(Object* p_obj) {
    if (type == ARRAY) { if (array_val) delete array_val; array_val = 0; }
    if (type == DICTIONARY) { if (dict_val) delete dict_val; dict_val = 0; }
    type = OBJECT;
    obj_val = p_obj;
    return *this;
}

Variant& Variant::operator=(const Array& p_array) {
    if (type == ARRAY) { if (array_val) delete array_val; }
    if (type == DICTIONARY) { if (dict_val) delete dict_val; dict_val = 0; }
    type = ARRAY;
    array_val = new Array(p_array);
    return *this;
}

Variant& Variant::operator=(const Dictionary& p_dict) {
    if (type == DICTIONARY) { if (dict_val) delete dict_val; }
    if (type == ARRAY) { if (array_val) delete array_val; array_val = 0; }
    type = DICTIONARY;
    dict_val = new Dictionary(p_dict);
    return *this;
}

Variant& Variant::operator=(const std::string& p_string) {
    if (type == ARRAY) { if (array_val) delete array_val; array_val = 0; }
    if (type == DICTIONARY) { if (dict_val) delete dict_val; dict_val = 0; }
    type = STRING; s_val = p_string; return *this;
}

}
