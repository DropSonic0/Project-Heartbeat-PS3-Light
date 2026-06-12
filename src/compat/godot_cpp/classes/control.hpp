#ifndef COMPAT_CONTROL_HPP
#define COMPAT_CONTROL_HPP

#include "canvas_item.hpp"

namespace godot {

class Control : public CanvasItem {
    GDCLASS(Control, CanvasItem);
};

}

#endif
