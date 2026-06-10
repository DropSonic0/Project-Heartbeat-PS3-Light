#ifndef COMPAT_VARIANT_HPP
#define COMPAT_VARIANT_HPP

#include <string>
#include <vector>
#include <map>

namespace godot {

class String : public std::string {
public:
    String() : std::string() {}
    String(const char* s) : std::string(s) {}
    String(const std::string& s) : std::string(s) {}
    static String num(double p_num) { return std::to_string(p_num).c_str(); }
    static String num_int64(long long p_num) { return std::to_string(p_num).c_str(); }
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
    String strip_edges() const { return *this; } // Placeholder
    String replace(const char* p_old, const char* p_new) const { return *this; } // Placeholder
    String to_lower() const { return *this; } // Placeholder
    long long to_int() const { return std::stoll(*this); }
    double to_float() const { return std::stod(*this); }
};

typedef std::vector<String> PackedStringArray;

class Variant {
public:
    enum Type { NIL, BOOL, INT, FLOAT, STRING, VECTOR2, COLOR, DICTIONARY, ARRAY, OBJECT };
    Variant() : type(NIL) {}
    Variant(bool p_bool) : type(BOOL) {}
    Variant(long long p_int) : type(INT) {}
    Variant(double p_float) : type(FLOAT) {}
    Variant(const String& p_string) : type(STRING) {}
    Type get_type() const { return type; }

    // Simplistic casting
    operator String() const { return ""; }
    operator long long() const { return 0; }
    operator double() const { return 0.0; }
    operator bool() const { return false; }

    template<class T>
    T* operator=(T* p_obj) { return p_obj; }

    Variant get(const String& p_name) const { return Variant(); }
    void call(const String& p_method, ...) {}

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
    void sort_custom(void* p_callable) {}
    void append_array(const Array& p_other) {
        for(size_t i=0; i<p_other.size(); i++) data.push_back(p_other[i]);
    }
};

class Dictionary {
    std::map<String, Variant> data;
public:
    bool has(const String& s) const { return data.find(s) != data.end(); }
    Variant& operator[](const String& s) { return data[s]; }
    Array keys() const {
        Array a;
        for (std::map<String, Variant>::const_iterator it = data.begin(); it != data.end(); ++it) {
            a.append(it->first);
        }
        return a;
    }
    void erase(const String& s) { data.erase(s); }
};

}

#endif
