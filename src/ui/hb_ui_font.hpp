#ifndef HB_UI_FONT_HPP
#define HB_UI_FONT_HPP

#include <godot_cpp/classes/font_variation.hpp>

namespace godot {

class HBUIFontNative : public FontVariation {
    GDCLASS(HBUIFontNative, FontVariation);

protected:
    static void _bind_methods();

public:
    enum FallbackHint {
        NORMAL,
        BOLD,
        BLACK
    };

    HBUIFontNative();
    ~HBUIFontNative();

    VARIANT_ENUM_CAST(FallbackHint);
};

} // namespace godot

#endif // HB_UI_FONT_HPP
