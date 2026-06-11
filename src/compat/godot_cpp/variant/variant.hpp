#ifndef COMPAT_VARIANT_HPP
#define COMPAT_VARIANT_HPP

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "../core/defs.hpp"

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
};

typedef std::vector<String> PackedStringArray;
typedef std::vector<uint8_t> PackedByteArray;
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
    Variant() : type(NIL) {}
    Variant(bool p_bool) : type(BOOL) {}
    Variant(int p_int) : type(INT) {}
    Variant(long long p_int) : type(INT) {}
    Variant(double p_float) : type(FLOAT) {}
    Variant(float p_float) : type(FLOAT) {}
    Variant(const String& p_string) : type(STRING) {}
    Variant(const char* p_string) : type(STRING) {}
    Variant(const Array& p_array);
    Variant(const Dictionary& p_dict);
    Variant(const Callable& p_callable);

    template <class T>
    Variant(const Ref<T> &p_ref) : type(OBJECT) {}

    Type get_type() const { return type; }

    operator String() const { return ""; }
    operator long long() const { return 0; }
    operator double() const { return 0.0; }
    operator float() const { return 0.0f; }
    operator int() const { return 0; }
    operator bool() const { return false; }
    operator Array() const;
    operator Dictionary() const;
    operator const void*() const { return NULL; }
    operator Object*() const;

    template <class T>
    operator Ref<T>() const;

    Variant& operator=(Object* p_obj);
    Variant& operator=(const Array& p_array);
    Variant& operator=(const Dictionary& p_dict);

    Variant get(const String& p_name) const { return Variant(); }
    Variant call(const String& p_method, ...);
    
    void sort_custom(const Variant& p_callable) {}

private:
    Type type;
};

class Array {
    std::vector<Variant> data;
public:
    bool is_empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
    void append(const Variant& p_val) { data.push_back(p_val); }
    void push_back(const Variant& p_val) { data.push_back(p_val); }
    void remove_at(size_t p_idx) { data.erase(data.begin() + p_idx); }
    void clear() { data.clear(); }
    Variant& operator[](size_t p_idx) { return data[p_idx]; }
    const Variant& operator[](size_t p_idx) const { return data[p_idx]; }
    int find(const Variant& p_val) const { return -1; }
    void sort_custom(const Variant& p_callable) {}
    void append_array(const Array& p_other) {
        for(size_t i=0; i<p_other.size(); i++) data.push_back(p_other[i]);
    }
};

class Dictionary {
    std::map<String, Variant> data;
public:
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
#include "../classes/object.hpp"

namespace godot {
inline Variant::Variant(const Array& p_array) : type(ARRAY) {}
inline Variant::Variant(const Dictionary& p_dict) : type(DICTIONARY) {}
inline Variant::Variant(const Callable& p_callable) : type(CALLABLE) {}

inline Variant::operator Array() const { return Array(); }
inline Variant::operator Dictionary() const { return Dictionary(); }
inline Variant::operator Object*() const { return NULL; }

template <class T>
inline Variant::operator Ref<T>() const { return Ref<T>(); }

inline Variant& Variant::operator=(Object* p_obj) { type = OBJECT; return *this; }
inline Variant& Variant::operator=(const Array& p_array) { type = ARRAY; return *this; }
inline Variant& Variant::operator=(const Dictionary& p_dict) { type = DICTIONARY; return *this; }

inline Variant Variant::call(const String& p_method, ...) { return Variant(); }
inline Variant Object::call(const char* p_method, ...) { return Variant(); }
}

#endif
