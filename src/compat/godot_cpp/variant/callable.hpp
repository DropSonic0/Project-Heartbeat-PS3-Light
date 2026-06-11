#ifndef COMPAT_CALLABLE_HPP
#define COMPAT_CALLABLE_HPP

#include "variant.hpp"

namespace godot {

class Callable {
public:
    Callable() {}
    template<class T>
    Callable(T* p_obj, const char* p_method) {}
};

}

#endif
