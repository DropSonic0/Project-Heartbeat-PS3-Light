#ifndef COMPAT_DEFS_HPP
#define COMPAT_DEFS_HPP

#define D_METHOD(m_name, ...) m_name
#define DEFVAL(m_val) m_val

#if defined(__PPU__) || defined(__SNC__) || (defined(_MSC_VER) && _MSC_VER <= 1700)
#define GD_OVERRIDE
#else
#define GD_OVERRIDE override
#endif

namespace godot {

template <class T>
class Ref {
    T *ptr;

public:
    Ref() : ptr(0) {}
    Ref(T *p_ptr) : ptr(p_ptr) {}
    T *operator->() { return ptr; }
    const T *operator->() const { return ptr; }
    T &operator*() { return *ptr; }
    const T &operator*() const { return *ptr; }
    bool is_valid() const { return ptr != 0; }
    bool is_null() const { return ptr == 0; }
    void instantiate() { ptr = new T(); }
    T* ptr() { return ptr; }
};

}

#endif
