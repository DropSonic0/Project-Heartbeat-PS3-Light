#ifndef COMPAT_NODE_HPP
#define COMPAT_NODE_HPP

#include "object.hpp"
#include "../variant/variant.hpp"

namespace godot {

class Node : public Object {
    GDCLASS(Node, Object);

public:
    virtual ~Node() {}
    void add_to_group(const String& p_group, bool p_persistent = false) {}
    void add_child(Node* p_child) {}
    virtual void _ready() {}
    virtual void _process(double p_delta) {}
    virtual void update() {}
    virtual void draw() {}
};

}

#endif
