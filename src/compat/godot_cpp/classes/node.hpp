#ifndef COMPAT_NODE_HPP
#define COMPAT_NODE_HPP

#include "object.hpp"
#include "../variant/variant.hpp"

namespace godot {

class Node : public Object {
    GDCLASS(Node, Object);

public:
    void add_to_group(const String& p_group, bool p_persistent = false) {}
};

}

#endif
