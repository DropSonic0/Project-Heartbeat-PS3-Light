#ifndef COMPAT_FONT_VARIATION_HPP
#define COMPAT_FONT_VARIATION_HPP

#include "ref_counted.hpp"
#include "../variant/variant.hpp"
#include "../variant/color.hpp"
#include "image.hpp"
#include <map>

namespace godot {

class FontVariation : public RefCounted {
    GDCLASS(FontVariation, RefCounted);
    PackedByteArray data;
    void* font_info = nullptr; // stbtt_fontinfo*

    struct CacheKey {
        String text;
        int size;

        bool operator<(const CacheKey& other) const {
            if (size != other.size) return size < other.size;
            return text < other.text;
        }
    };
    mutable std::map<CacheKey, Ref<Image> > text_cache;

public:
    FontVariation();
    ~FontVariation();

    void set_data(const PackedByteArray& p_data);
    PackedByteArray get_data() const { return data; }
    
    Ref<Image> render_text(const String& p_text, int p_font_size) const;
    void clear_cache();
};

}

#endif
