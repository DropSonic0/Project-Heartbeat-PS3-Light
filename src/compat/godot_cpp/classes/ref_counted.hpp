#ifndef COMPAT_REF_COUNTED_HPP
#define COMPAT_REF_COUNTED_HPP

#include "object.hpp"

namespace godot {

class RefCounted : public Object {
    GDCLASS(RefCounted, Object);
};

}

#endif
