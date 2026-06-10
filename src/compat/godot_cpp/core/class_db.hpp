#ifndef COMPAT_CLASS_DB_HPP
#define COMPAT_CLASS_DB_HPP

#include "../classes/object.hpp"
#include <string>

namespace godot {

class ClassDB {
public:
    template<class T, class M>
    static void bind_method(const char* p_method, M p_func) {}

    template<class T, class M>
    static void bind_static_method(const char* p_class, const char* p_method, M p_func) {}

    static void add_property(const char* p_class, const void* p_info, const char* p_set, const char* p_get) {}
};

#define ADD_PROPERTY(m_info, m_set, m_get)
#define BIND_ENUM_CONSTANT(m_constant)
#define VARIANT_ENUM_CAST(m_enum)

}

#endif
