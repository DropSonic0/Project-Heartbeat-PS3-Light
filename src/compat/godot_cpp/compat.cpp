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

Variant Object::call(const char* p_method, ...) {
    UtilityFunctions::print(std::string("Object::call: ") + p_method);
    return Variant();
}

}
