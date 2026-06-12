#ifndef COMPAT_REG_EX_HPP
#define COMPAT_REG_EX_HPP

#include "ref_counted.hpp"
#include "../variant/variant.hpp"

namespace godot {

class RegEx : public RefCounted {
    GDCLASS(RegEx, RefCounted);

public:
    Error compile(const String &p_pattern) { return OK; }
    String sub(const String &p_text, const String &p_replacement, bool p_all = false) const { return p_text; }
};

}

#endif
