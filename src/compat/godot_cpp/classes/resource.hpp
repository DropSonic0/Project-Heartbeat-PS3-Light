#ifndef COMPAT_RESOURCE_HPP
#define COMPAT_RESOURCE_HPP

#include "ref_counted.hpp"

namespace godot {

class Resource : public RefCounted {
    GDCLASS(Resource, RefCounted);
};

}

#endif
