#ifndef HB_UI_STYLEBOX_FLAT_HPP
#define HB_UI_STYLEBOX_FLAT_HPP

#include <godot_cpp/classes/style_box_flat.hpp>

namespace godot {

class HBUIStyleboxFlatNative : public StyleBoxFlat {
    GDCLASS(HBUIStyleboxFlatNative, StyleBoxFlat);

protected:
    static void _bind_methods();

public:
    HBUIStyleboxFlatNative();
    ~HBUIStyleboxFlatNative();
};

} // namespace godot

#endif // HB_UI_STYLEBOX_FLAT_HPP
