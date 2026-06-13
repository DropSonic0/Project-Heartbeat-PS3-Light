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
    String lpad(int p_len, const char* p_char = " ") const {
        if ((int)size() >= p_len) return *this;
        int pad_len = p_len - (int)size();
        std::string pad_str = "";
        std::string p_char_str = p_char;
        if (p_char_str.empty()) return *this;
        while ((int)pad_str.size() < pad_len) {
            pad_str += p_char_str;
        }
        return String(pad_str.substr(0, pad_len) + *this);
    }
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
    String strip_edges() const {
        size_t first = find_first_not_of(" \t\n\r");
        if (std::string::npos == first) return String("");
        size_t last = find_last_not_of(" \t\n\r");
        return String(this->std::string::substr(first, (last - first + 1)));
    }
    String replace(const char* p_old, const char* p_new) const {
        std::string res = *this;
        std::string from = p_old;
        std::string to = p_new;
        if (from.empty()) return *this;
        size_t start_pos = 0;
        while((start_pos = res.find(from, start_pos)) != std::string::npos) {
            res.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return String(res);
    }
    String to_lower() const {
        std::string res = *this;
        for (size_t i = 0; i < res.size(); i++) {
            if (res[i] >= 'A' && res[i] <= 'Z') res[i] += 32;
        }
        return String(res);
    }
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
class PackedStringArray : public std::vector<String> {
public:
    PackedStringArray() {}
    PackedStringArray(const std::vector<String>& p_vec) : std::vector<String>(p_vec) {}
    void append(const String& p_string) { push_back(p_string); }
};
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

    operator String() const {
        if (type == STRING) return s_val;
        if (type == INT) return String::num_int64(i_val);
        if (type == FLOAT) return String::num(f_val);
        if (type == BOOL) return i_val ? "true" : "false";
        return "";
    }
    operator long long() const { return (type == FLOAT) ? (long long)f_val : i_val; }
    operator double() const { return (type == INT || type == BOOL) ? (double)i_val : f_val; }
    operator float() const { return (type == INT || type == BOOL) ? (float)i_val : (float)f_val; }
    operator int() const { return (type == FLOAT) ? (int)f_val : (int)i_val; }
    operator bool() const {
        if (type == BOOL || type == INT) return i_val != 0;
        if (type == FLOAT) return f_val != 0.0;
        if (type == STRING) return !s_val.empty();
        if (type == OBJECT) return obj_val != 0;
        return false;
    }
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

    Variant get(const String& p_name) const;
    Variant call(const String& p_method, const Variant& p_arg1 = Variant(), const Variant& p_arg2 = Variant());
    
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
    struct Data {
        std::vector<Variant> vec;
        int refcount;
    } *data;
    void _ref(const Array& p_other) {
        data = p_other.data;
        if (data) data->refcount++;
    }
    void _unref() {
        if (data) {
            data->refcount--;
            if (data->refcount <= 0) {
                delete data;
            }
            data = 0;
        }
    }
public:
    Array() {
        data = new Data;
        data->refcount = 1;
    }
    Array(const Array& p_other) { _ref(p_other); }
    Array& operator=(const Array& p_other) {
        if (this == &p_other) return *this;
        _unref();
        _ref(p_other);
        return *this;
    }
    ~Array() { _unref(); }

    Array(const std::vector<String>& p_vec) {
        data = new Data;
        data->refcount = 1;
        for (size_t i = 0; i < p_vec.size(); i++) {
            data->vec.push_back(p_vec[i]);
        }
    }
    bool is_empty() const { return data->vec.empty(); }
    size_t size() const { return data->vec.size(); }
    void append(const Variant& p_val) { data->vec.push_back(p_val); }
    void push_back(const Variant& p_val) { data->vec.push_back(p_val); }
    Variant pop_back() {
        if (data->vec.empty()) return Variant();
        Variant v = data->vec.back();
        data->vec.pop_back();
        return v;
    }
    void remove_at(size_t p_idx) { data->vec.erase(data->vec.begin() + p_idx); }
    void clear() { data->vec.clear(); }
    Variant& operator[](size_t p_idx) { return data->vec[p_idx]; }
    const Variant& operator[](size_t p_idx) const { return data->vec[p_idx]; }
    int find(const Variant& p_val) const {
        for (size_t i = 0; i < data->vec.size(); i++) {
            if (data->vec[i] == p_val) return (int)i;
        }
        return -1;
    }
    int bsearch(const Variant& p_val, bool p_before = true) const {
        int low = 0;
        int high = (int)data->vec.size();
        while (low < high) {
            int mid = low + (high - low) / 2;
            if (p_before) {
                if (data->vec[mid] < p_val) low = mid + 1;
                else high = mid;
            } else {
                if (p_val < data->vec[mid]) high = mid;
                else low = mid + 1;
            }
        }
        return low;
    }
    void sort_custom(const Variant& p_callable) {}
    void append_array(const Array& p_other) {
        for(size_t i=0; i<p_other.size(); i++) data->vec.push_back(p_other[i]);
    }
};

class Dictionary {
    struct Data {
        std::map<String, Variant> map;
        int refcount;
    } *data;
    void _ref(const Dictionary& p_other) {
        data = p_other.data;
        if (data) data->refcount++;
    }
    void _unref() {
        if (data) {
            data->refcount--;
            if (data->refcount <= 0) {
                delete data;
            }
            data = 0;
        }
    }
public:
    Dictionary() {
        data = new Data;
        data->refcount = 1;
    }
    Dictionary(const Dictionary& p_other) { _ref(p_other); }
    Dictionary& operator=(const Dictionary& p_other) {
        if (this == &p_other) return *this;
        _unref();
        _ref(p_other);
        return *this;
    }
    ~Dictionary() { _unref(); }

    bool has(const String& s) const { return data->map.find(s) != data->map.end(); }
    bool is_empty() const { return data->map.empty(); }
    Variant& operator[](const String& s) { return data->map[s]; }
    Variant& operator[](const char* s) { return data->map[String(s)]; }
    Variant& operator[](int p_idx) { return data->map[String::num_int64(p_idx)]; }
    Variant& operator[](long long p_idx) { return data->map[String::num_int64(p_idx)]; }
    Variant& operator[](const Variant& p_key) { return data->map[String(p_key)]; }
    const Variant& operator[](const String& s) const {
        static Variant nil;
        std::map<String, Variant>::const_iterator it = data->map.find(s);
        return it != data->map.end() ? it->second : nil;
    }
    const Variant& operator[](const char* s) const { return (*this)[String(s)]; }
    const Variant& operator[](int p_idx) const { return (*this)[String::num_int64(p_idx)]; }
    const Variant& operator[](long long p_idx) const { return (*this)[String::num_int64(p_idx)]; }
    const Variant& operator[](const Variant& p_key) const { return (*this)[String(p_key)]; }
    Array keys() const {
        Array a;
        for (std::map<String, Variant>::const_iterator it = data->map.begin(); it != data->map.end(); ++it) {
            a.append(it->first);
        }
        return a;
    }
    void erase(const String& s) { data->map.erase(s); }
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


}

#endif
