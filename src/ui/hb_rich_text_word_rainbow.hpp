#ifndef HB_RICH_TEXT_WORD_RAINBOW_HPP
#define HB_RICH_TEXT_WORD_RAINBOW_HPP

#include <godot_cpp/classes/rich_text_effect.hpp>

namespace godot {

class RichTextWordRainbowNative : public RichTextEffect {
    GDCLASS(RichTextWordRainbowNative, RichTextEffect);

protected:
    static void _bind_methods();

public:
    RichTextWordRainbowNative();
    ~RichTextWordRainbowNative();
};

} // namespace godot

#endif // HB_RICH_TEXT_WORD_RAINBOW_HPP
