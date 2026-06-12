#ifndef COMPAT_VARIANT_HPP
#define COMPAT_VARIANT_HPP

#include "../core/defs.hpp"
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <stdlib.h>

namespace godot {

class String : public std::string {
public:
    String() : std::string() {}
    String(const char* s) : std::string(s) {}
    String(const std::string& s) : std::string(s) {}
    static String num(double p_num) {
        std::stringstream ss;
        ss << p_num;
        return ss.str().c_str();
    }
    static String num_int64(long long p_num) {
        std::stringstream ss;
        ss << p_num;
        return ss.str().c_str();
    }
    String substr(size_t p_from, size_t p_chars = (size_t)std::string::npos) const {
        return ((const std::string*)this)->std::string::substr(p_from, p_chars).c_str();
    }
    String lpad(int p_len, const char* p_char) { return *this; }
    bool is_empty() const { return empty(); }
    bool begins_with(const char* s) const { return find(s) == 0; }
    bool ends_with(const char* s) const { return size() >= std::string(s).size() && rfind(s) == size() - std::string(s).size(); }
    std::vector<String> split(const char* p_splitter) const {
        std::vector<String> res;
        size_t start = 0, end;
        while ((end = find(p_splitter, start)) != std::string::npos) {
            res.push_back(substr(start, end - start));
            start = end + std::string(p_splitter).length();
        }
        res.push_back(substr(start));
        return res;
    }
    String strip_edges() const { return *this; }
    String replace(const char* p_old, const char* p_new) const { return *this; }
    String to_lower() const { return *this; }
    long long to_int() const {
        std::stringstream ss(*this);
        long long res;
        ss >> res;
        return res;
    }
    double to_float() const {
        std::stringstream ss(*this);
        double res;
        ss >> res;
        return res;
    }
    bool is_valid_int() const {
        if (empty()) return false;
        char *endptr;
        strtoll(c_str(), &endptr, 10);
        return *endptr == '\0';
    }
    bool is_valid_float() const {
        if (empty()) return false;
        char *endptr;
        strtod(c_str(), &endptr);
        return *endptr == '\0';
    }
};

typedef std::vector<unsigned char> PackedByteArray;
typedef std::vector<String> PackedStringArray;
typedef std::vector<int32_t> PackedInt32Array;
typedef std::vector<int64_t> PackedInt64Array;
typedef std::vector<float> PackedFloat32Array;
typedef std::vector<double> PackedFloat64Array;

class Array;
class Dictionary;
class Callable;
class Object;
template <class T> class Ref;

class Variant {
public:
    enum Type { NIL, BOOL, INT, FLOAT, STRING, VECTOR2, COLOR, DICTIONARY, ARRAY, OBJECT, CALLABLE };
    Variant() : type(NIL), i_val(0), f_val(0.0), array_val(0), dict_val(0), obj_val(0) {}
    Variant(bool p_bool) : type(BOOL), i_val(p_bool ? 1 : 0), f_val(0.0), array_val(0), dict_val(0), obj_val(0) {}
    Variant(int p_int) : type(INT), i_val(p_int), f_val(0.0), array_val(0), dict_val(0), obj_val(0) {}
    Variant(long long p_int) : type(INT), i_val(p_int), f_val(0.0), array_val(0), dict_val(0), obj_val(0) {}
    Variant(double p_float) : type(FLOAT), i_val(0), f_val(p_float), array_val(0), dict_val(0), obj_val(0) {}
    Variant(float p_float) : type(FLOAT), i_val(0), f_val(p_float), array_val(0), dict_val(0), obj_val(0) {}
    Variant(const String& p_string) : type(STRING), s_val(p_string), i_val(0), f_val(0.0), array_val(0), dict_val(0), obj_val(0) {}
    Variant(const char* p_string) : type(STRING), s_val(p_string), i_val(0), f_val(0.0), array_val(0), dict_val(0), obj_val(0) {}
    Variant(const Array& p_array);
    Variant(const Dictionary& p_dict);
    Variant(const Callable& p_callable);

    template <class T>
    Variant(const Ref<T> &p_ref) : type(OBJECT), i_val(0), f_val(0.0), array_val(0), dict_val(0), obj_val((Object*)p_ref.ptr()) {}

    Variant(const Variant& p_other);
    ~Variant();

    Type get_type() const { return type; }

    operator String() const { return s_val; }
    operator long long() const { return (type == FLOAT) ? (long long)f_val : i_val; }
    operator double() const { return (type == INT || type == BOOL) ? (double)i_val : f_val; }
    operator float() const { return (type == INT || type == BOOL) ? (float)i_val : (float)f_val; }
    operator int() const { return (type == FLOAT) ? (int)f_val : (int)i_val; }
    operator bool() const { return i_val != 0 || f_val != 0.0 || obj_val != 0; }
    operator Array() const;
    operator Dictionary() const;
    operator const void*() const { return (const void*)obj_val; }
    operator Object*() const { return obj_val; }

    template <class T>
    operator Ref<T>() const;

    Variant& operator=(const Variant& p_other);
    Variant& operator=(Object* p_obj);
    Variant& operator=(const Array& p_array);
    Variant& operator=(const Dictionary& p_dict);
    Variant& operator=(const std::string& p_string);

    Variant get(const String& p_name) const { return Variant(); }
    Variant call(const String& p_method, ...);
    
    void sort_custom(const Variant& p_callable) {}

    bool operator==(const Variant& p_other) const {
        if (type != p_other.type) return false;
        if (type == INT) return i_val == p_other.i_val;
        if (type == FLOAT) return f_val == p_other.f_val;
        if (type == STRING) return s_val == p_other.s_val;
        if (type == OBJECT) return obj_val == p_other.obj_val;
        return true; // Simplified
    }
    bool operator<(const Variant& p_other) const {
        if (type == INT && p_other.type == INT) return i_val < p_other.i_val;
        if (type == FLOAT && p_other.type == FLOAT) return f_val < p_other.f_val;
        if (type == STRING && p_other.type == STRING) return s_val < p_other.s_val;
        return false;
    }

private:
    Type type;
    String s_val;
    long long i_val;
    double f_val;
    Array *array_val;
    Dictionary *dict_val;
    Object *obj_val;
};

class Array {
public:
    std::vector<Variant> data;
    Array() {}
    Array(const std::vector<String>& p_vec) {
        for (size_t i = 0; i < p_vec.size(); i++) {
            data.push_back(p_vec[i]);
        }
    }
    bool is_empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
    void append(const Variant& p_val) { data.push_back(p_val); }
    void push_back(const Variant& p_val) { data.push_back(p_val); }
    Variant pop_back() {
        if (data.empty()) return Variant();
        Variant v = data.back();
        data.pop_back();
        return v;
    }
    void remove_at(size_t p_idx) { data.erase(data.begin() + p_idx); }
    void clear() { data.clear(); }
    Variant& operator[](size_t p_idx) { return data[p_idx]; }
    const Variant& operator[](size_t p_idx) const { return data[p_idx]; }
    int find(const Variant& p_val) const {
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i] == p_val) return (int)i;
        }
        return -1;
    }
    int bsearch(const Variant& p_val, bool p_before = true) const {
        int low = 0;
        int high = (int)data.size();
        while (low < high) {
            int mid = low + (high - low) / 2;
            if (p_before) {
                if (data[mid] < p_val) low = mid + 1;
                else high = mid;
            } else {
                if (p_val < data[mid]) high = mid;
                else low = mid + 1;
            }
        }
        return low;
    }
    void sort_custom(const Variant& p_callable) {}
    void append_array(const Array& p_other) {
        for(size_t i=0; i<p_other.size(); i++) data.push_back(p_other[i]);
    }
};

class Dictionary {
public:
    std::map<String, Variant> data;
    bool has(const String& s) const { return data.find(s) != data.end(); }
    bool is_empty() const { return data.empty(); }
    Variant& operator[](const String& s) { return data[s]; }
    Variant& operator[](const char* s) { return data[String(s)]; }
    Variant& operator[](int p_idx) { return data[String::num_int64(p_idx)]; }
    Variant& operator[](long long p_idx) { return data[String::num_int64(p_idx)]; }
    Variant& operator[](const Variant& p_key) { return data[String(p_key)]; }
    const Variant& operator[](const String& s) const {
        static Variant nil;
        std::map<String, Variant>::const_iterator it = data.find(s);
        return it != data.end() ? it->second : nil;
    }
    const Variant& operator[](const char* s) const { return (*this)[String(s)]; }
    const Variant& operator[](int p_idx) const { return (*this)[String::num_int64(p_idx)]; }
    const Variant& operator[](long long p_idx) const { return (*this)[String::num_int64(p_idx)]; }
    const Variant& operator[](const Variant& p_key) const { return (*this)[String(p_key)]; }
    Array keys() const {
        Array a;
        for (std::map<String, Variant>::const_iterator it = data.begin(); it != data.end(); ++it) {
            a.append(it->first);
        }
        return a;
    }
    void erase(const String& s) { data.erase(s); }
};

} // namespace godot

#include "callable.hpp"

namespace godot {
inline Variant::Variant(const Array& p_array) : type(ARRAY), i_val(0), f_val(0.0), dict_val(0), obj_val(0) {
    array_val = new Array(p_array);
}
inline Variant::Variant(const Dictionary& p_dict) : type(DICTIONARY), i_val(0), f_val(0.0), array_val(0), obj_val(0) {
    dict_val = new Dictionary(p_dict);
}
inline Variant::Variant(const Callable& p_callable) : type(CALLABLE), i_val(0), f_val(0.0), array_val(0), dict_val(0), obj_val(0) {}

inline Variant::Variant(const Variant& p_other) : type(NIL), array_val(0), dict_val(0), obj_val(0) {
    *this = p_other;
}

inline Variant::~Variant() {
    if (array_val) delete array_val;
    if (dict_val) delete dict_val;
}

inline Variant::operator Array() const { return array_val ? *array_val : Array(); }
inline Variant::operator Dictionary() const { return dict_val ? *dict_val : Dictionary(); }

template <class T>
inline Variant::operator Ref<T>() const { return Ref<T>((T*)obj_val); }

inline Variant& Variant::operator=(const Variant& p_other) {
    if (this == &p_other) return *this;
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    type = p_other.type;
    s_val = p_other.s_val;
    i_val = p_other.i_val;
    f_val = p_other.f_val;
    obj_val = p_other.obj_val;
    if (p_other.array_val) array_val = new Array(*p_other.array_val);
    if (p_other.dict_val) dict_val = new Dictionary(*p_other.dict_val);
    return *this;
}

inline Variant& Variant::operator=(Object* p_obj) {
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    type = OBJECT;
    obj_val = p_obj;
    return *this;
}
inline Variant& Variant::operator=(const Array& p_array) {
    if (array_val) delete array_val;
    if (dict_val) { delete dict_val; dict_val = 0; }
    type = ARRAY;
    array_val = new Array(p_array);
    return *this;
}
inline Variant& Variant::operator=(const Dictionary& p_dict) {
    if (dict_val) delete dict_val;
    if (array_val) { delete array_val; array_val = 0; }
    type = DICTIONARY;
    dict_val = new Dictionary(p_dict);
    return *this;
}
inline Variant& Variant::operator=(const std::string& p_string) {
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    type = STRING; s_val = p_string; return *this;
}

inline Variant Variant::call(const String& p_method, ...) { return Variant(); }
}

#endif
