#include "classes/object.hpp"
#include "variant/variant.hpp"
#include "variant/utility_functions.hpp"
#include "classes/project_settings.hpp"
#include "../../utils/hb_pck_reader.hpp"
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
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    if (packed_byte_array_val) { delete packed_byte_array_val; packed_byte_array_val = 0; }
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
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    if (packed_byte_array_val) { delete packed_byte_array_val; packed_byte_array_val = 0; }
    type = OBJECT;
    obj_val = p_obj;
    return *this;
}

Variant& Variant::operator=(const Array& p_array) {
    if (array_val) { delete array_val; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    if (packed_byte_array_val) { delete packed_byte_array_val; packed_byte_array_val = 0; }
    type = ARRAY;
    array_val = new Array(p_array);
    return *this;
}

Variant& Variant::operator=(const Dictionary& p_dict) {
    if (dict_val) { delete dict_val; }
    if (array_val) { delete array_val; array_val = 0; }
    if (packed_byte_array_val) { delete packed_byte_array_val; packed_byte_array_val = 0; }
    type = DICTIONARY;
    dict_val = new Dictionary(p_dict);
    return *this;
}

Variant& Variant::operator=(const std::string& p_string) {
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    if (packed_byte_array_val) { delete packed_byte_array_val; packed_byte_array_val = 0; }
    type = STRING; s_val = p_string; return *this;
}

Variant& Variant::operator=(long p_int) {
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    if (packed_byte_array_val) { delete packed_byte_array_val; packed_byte_array_val = 0; }
    type = INT; i_val = p_int; return *this;
}

Variant& Variant::operator=(double p_float) {
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    if (packed_byte_array_val) { delete packed_byte_array_val; packed_byte_array_val = 0; }
    type = FLOAT; f_val = p_float; return *this;
}

Variant& Variant::operator=(float p_float) {
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    if (packed_byte_array_val) { delete packed_byte_array_val; packed_byte_array_val = 0; }
    type = FLOAT; f_val = p_float; return *this;
}

Variant& Variant::operator=(long long p_int) {
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    if (packed_byte_array_val) { delete packed_byte_array_val; packed_byte_array_val = 0; }
    type = INT; i_val = p_int; return *this;
}

Variant& Variant::operator=(int p_int) {
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    if (packed_byte_array_val) { delete packed_byte_array_val; packed_byte_array_val = 0; }
    type = INT; i_val = p_int; return *this;
}

Variant& Variant::operator=(const char* p_string) {
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    if (packed_byte_array_val) { delete packed_byte_array_val; packed_byte_array_val = 0; }
    type = STRING; s_val = p_string; return *this;
}

Variant& Variant::operator=(const PackedByteArray& p_packed_byte_array) {
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    if (packed_byte_array_val) { delete packed_byte_array_val; }
    type = PACKED_BYTE_ARRAY;
    packed_byte_array_val = new PackedByteArray(p_packed_byte_array);
    return *this;
}

bool ProjectSettings::load_resource_pack(const String& p_pack, bool p_replace_files, int p_offset) {
    UtilityFunctions::print("ProjectSettings: Loading resource pack: " + p_pack);
    PCKReader* reader = new PCKReader();
    if (reader->load_pck(p_pack)) {
        loaded_packs.push_back(reader);
        return true;
    }
    delete reader;
    return false;
}

bool ProjectSettings::is_pack_loaded(const String& p_pack) const {
    for (size_t i = 0; i < loaded_packs.size(); i++) {
        if (loaded_packs[i]->get_pck_path() == p_pack) return true;
    }
    return false;
}

PCKFileEntry ProjectSettings::find_file_in_packs(const String& p_path, String& r_pck_path) const {
    for (int i = (int)loaded_packs.size() - 1; i >= 0; i--) {
        if (loaded_packs[i]->file_exists(p_path)) {
            r_pck_path = loaded_packs[i]->get_pck_path();
            return loaded_packs[i]->get_file_entry(p_path);
        }
    }
    return {0, 0};
}

PackedStringArray ProjectSettings::get_files_in_packs(const String& p_path) const {
    PackedStringArray res;
    String prefix = p_path;
    if (!prefix.ends_with("/")) prefix += "/";

    for (size_t i = 0; i < loaded_packs.size(); i++) {
        std::vector<String> all_files = loaded_packs[i]->get_all_files();
        for (size_t j = 0; j < all_files.size(); j++) {
            if (all_files[j].begins_with(prefix.c_str())) {
                String sub = all_files[j].substr(prefix.length());
                if (sub.find("/") == std::string::npos) {
                    res.append(sub);
                }
            }
        }
    }
    return res;
}

PackedStringArray ProjectSettings::get_directories_in_packs(const String& p_path) const {
    PackedStringArray res;
    String prefix = p_path;
    if (!prefix.ends_with("/")) prefix += "/";

    for (size_t i = 0; i < loaded_packs.size(); i++) {
        std::vector<String> all_files = loaded_packs[i]->get_all_files();
        for (size_t j = 0; j < all_files.size(); j++) {
            if (all_files[j].begins_with(prefix.c_str())) {
                String sub = all_files[j].substr(prefix.length());
                size_t slash_pos = sub.find("/");
                if (slash_pos != std::string::npos) {
                    String dir_name = sub.substr(0, slash_pos);
                    bool already_has = false;
                    for (int k=0; k<res.size(); k++) if (res[k] == dir_name) already_has = true;
                    if (!already_has) res.append(dir_name);
                }
            }
        }
    }
    return res;
}

}
