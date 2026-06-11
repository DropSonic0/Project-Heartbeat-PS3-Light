#ifndef COMPAT_CLASS_DB_HPP
#define COMPAT_CLASS_DB_HPP

#include "defs.hpp"
#include "../classes/object.hpp"
#include <string>

#ifndef D_METHOD
#define D_METHOD(m_name, ...) m_name
#endif
#ifndef DEFVAL
#define DEFVAL(m_val) m_val
#endif

namespace godot {

class ClassDB {
public:
    template<class M>
    static void bind_method(const char* p_method, M p_func) {}
    
    template<class M, class D>
    static void bind_method(const char* p_method, M p_func, D p_def) {}

    template<class M>
    static void bind_static_method(const char* p_class, const char* p_method, M p_func) {}
    
    template<class M, class D>
    static void bind_static_method(const char* p_class, const char* p_method, M p_func, D p_def) {}

    static void add_property(const char* p_class, const void* p_info, const char* p_set, const char* p_get) {}
};

struct PropertyInfo {
    PropertyInfo(int p_type, const char* p_name) {}
    PropertyInfo(int p_type, const char* p_name, int p_hint, const char* p_hint_string) {}
};

#define PROPERTY_HINT_ENUM 0

#define ADD_PROPERTY(m_info, m_set, m_get)
#define BIND_ENUM_CONSTANT(m_constant)

}

#endif
