#ifndef COMPAT_REF_COUNTED_HPP
#define COMPAT_REF_COUNTED_HPP

#include "object.hpp"

namespace godot {

class RefCounted : public Object {
    GDCLASS(RefCounted, Object);
    int ref_count = 0;

public:
    bool reference() {
        ref_count++;
        return true;
    }

    bool unreference() {
        ref_count--;
        return ref_count <= 0;
    }

    int get_reference_count() const {
        return ref_count;
    }
    virtual bool is_ref_counted() const override { return true; }
};

}

#endif
