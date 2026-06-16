#ifndef COMPAT_RID_HPP
#define COMPAT_RID_HPP

#include <stdint.h>

namespace godot {

class RID {
    uint64_t id;
public:
    RID() : id(0) {}
    bool is_valid() const { return id != 0; }
    uint64_t get_id() const { return id; }
    bool operator==(const RID& p_rid) const { return id == p_rid.id; }
    bool operator!=(const RID& p_rid) const { return id != p_rid.id; }
    bool operator<(const RID& p_rid) const { return id < p_rid.id; }
};

}

#endif
