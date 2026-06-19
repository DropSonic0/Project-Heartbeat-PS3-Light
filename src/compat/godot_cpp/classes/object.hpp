#ifndef COMPAT_OBJECT_HPP
#define COMPAT_OBJECT_HPP

#include <string>
#include <vector>
#include "../core/defs.hpp"

namespace godot {

class Variant;
class Object {
public:
    virtual ~Object();
    virtual void set(const std::string &p_name, const Variant& p_value);
    virtual Variant get(const std::string &p_name) const;
    
    // Legacy stubs
    virtual void set_ptr(const std::string &p_name, const void* p_value) {}
    virtual void* get_ptr(const std::string &p_name) const { return NULL; }
    
    virtual bool has_method(const std::string &p_method) const;
    virtual Variant call(const std::string &p_method, const Variant& p_arg1, const Variant& p_arg2);
    static void _bind_methods() {}
};

#define GDCLASS(m_class, m_inherits) \
public: \
    typedef m_inherits Inherits; \
    static const char* get_class_static() { return #m_class; } \
    static void _bind_methods_shim() { m_class::_bind_methods(); }

#define GDREGISTER_CLASS(m_class) \
    godot::UtilityFunctions::print("Registering class: " #m_class); \
    m_class::_bind_methods_shim();

}

#endif
