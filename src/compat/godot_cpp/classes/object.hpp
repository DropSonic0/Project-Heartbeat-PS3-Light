#ifndef COMPAT_OBJECT_HPP
#define COMPAT_OBJECT_HPP

#include <string>
#include <map>
#include <vector>

namespace godot {

class Variant;
class Object {
public:
    virtual ~Object() {}
    virtual void set(const std::string &p_name, const void* p_value) {}
    virtual void* get(const std::string &p_name) const { return NULL; }
    virtual bool has_method(const std::string &p_method) const { return false; }
    virtual Variant call(const char* p_method, ...);
};

#define GDCLASS(m_class, m_inherits) \
public: \
    typedef m_inherits Inherits; \
    static const char* get_class_static() { return #m_class; }

#define GDREGISTER_CLASS(m_class)

}

#endif
