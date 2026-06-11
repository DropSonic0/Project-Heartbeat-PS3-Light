#ifndef COMPAT_JSON_HPP
#define COMPAT_JSON_HPP

#include "ref_counted.hpp"
#include "../variant/variant.hpp"

namespace godot {

class JSON : public RefCounted {
    GDCLASS(JSON, RefCounted);

public:
    int parse(const String& p_json_string) { return 0; }
    Variant get_data() const { return Variant(); }
};

}

#endif
