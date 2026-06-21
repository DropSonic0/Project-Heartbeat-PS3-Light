#include "classes/object.hpp"
#include "variant/variant.hpp"
#include "variant/utility_functions.hpp"
#include "classes/project_settings.hpp"
#include "../../utils/hb_pck_reader.hpp"
#include "../../threads/scoped_lock.hpp"
#include "../../threads/mutex.hpp"
#include <map>
#include <string>

namespace godot {

static std::map<const Object*, std::map<std::string, Variant> > _object_properties;
static Threads::Mutex _object_properties_mutex;

Object::~Object() {
    std::map<std::string, Variant> props;
    {
        Threads::ScopedLock lock(_object_properties_mutex);
        std::map<const Object*, std::map<std::string, Variant> >::iterator it = _object_properties.find(this);
        if (it != _object_properties.end()) {
            props = it->second;
            _object_properties.erase(it);
        }
    }
    // Variant clear can delete other objects, so we clear properties outside the lock
    props.clear();
}

void Object::set(const std::string &p_name, const Variant& p_value) {
    Threads::ScopedLock lock(_object_properties_mutex);
    _object_properties[this][p_name] = p_value;
}

Variant Object::get(const std::string &p_name) const {
    Threads::ScopedLock lock(_object_properties_mutex);
    std::map<const Object*, std::map<std::string, Variant> >::iterator it = _object_properties.find(this);
    if (it != _object_properties.end()) {
        std::map<std::string, Variant>::iterator it2 = it->second.find(p_name);
        if (it2 != it->second.end()) {
            return it2->second;
        }
    }
    return Variant();
}



void Variant::_ref_obj(Object* p_obj) {
    obj_val = p_obj;
    if (obj_val && obj_val->is_ref_counted()) {
        ((RefCounted*)obj_val)->reference();
    }
}

void Variant::_unref_obj() {
    if (type == OBJECT && obj_val && obj_val->is_ref_counted()) {
        if (((RefCounted*)obj_val)->unreference()) {
            delete obj_val;
        }
    }
    obj_val = 0;
}

void Variant::_clear() {
    if (array_val) { delete array_val; array_val = 0; }
    if (dict_val) { delete dict_val; dict_val = 0; }
    if (packed_byte_array_val) { delete packed_byte_array_val; packed_byte_array_val = 0; }
    _unref_obj();
    type = NIL;
    i_val = 0;
    f_val = 0.0;
    s_val = "";
}

Variant::Variant(const Variant& p_other) : type(NIL), array_val(0), dict_val(0), obj_val(0), packed_byte_array_val(0) {
    *this = p_other;
}

Variant::~Variant() {
    _clear();
}

bool Object::has_method(const std::string &p_method) const {
    return false;
}

Variant Object::call(const std::string &p_method, const Variant& p_arg1, const Variant& p_arg2) {
    UtilityFunctions::print("Object::call: " + p_method);
    return Variant();
}

Variant Variant::get(const String& p_name) const {
    if (type == OBJECT && obj_val) {
        return obj_val->get(p_name);
    }
    return Variant();
}

Variant Variant::call(const String& p_method, const Variant& p_arg1, const Variant& p_arg2) {
    if (type == OBJECT && obj_val) {
        return obj_val->call(p_method, p_arg1, p_arg2);
    }
    return Variant();
}

Variant& Variant::operator=(const Variant& p_other) {
    if (this == &p_other) return *this;
    _clear();
    type = p_other.type;
    s_val = p_other.s_val;
    i_val = p_other.i_val;
    f_val = p_other.f_val;
    _ref_obj(p_other.obj_val);
    v2_val[0] = p_other.v2_val[0];
    v2_val[1] = p_other.v2_val[1];
    if (p_other.array_val) array_val = new Array(*p_other.array_val);
    if (p_other.dict_val) dict_val = new Dictionary(*p_other.dict_val);
    if (p_other.packed_byte_array_val) packed_byte_array_val = new PackedByteArray(*p_other.packed_byte_array_val);
    return *this;
}

Variant& Variant::operator=(Object* p_obj) {
    _clear();
    type = OBJECT;
    _ref_obj(p_obj);
    return *this;
}

Variant& Variant::operator=(const Array& p_array) {
    _clear();
    type = ARRAY;
    array_val = new Array(p_array);
    return *this;
}

Variant& Variant::operator=(const Dictionary& p_dict) {
    _clear();
    type = DICTIONARY;
    dict_val = new Dictionary(p_dict);
    return *this;
}

Variant& Variant::operator=(const std::string& p_string) {
    _clear();
    type = STRING; s_val = p_string; return *this;
}

Variant& Variant::operator=(long p_int) {
    _clear();
    type = INT; i_val = p_int; return *this;
}

Variant& Variant::operator=(double p_float) {
    _clear();
    type = FLOAT; f_val = p_float; return *this;
}

Variant& Variant::operator=(float p_float) {
    _clear();
    type = FLOAT; f_val = p_float; return *this;
}

Variant& Variant::operator=(long long p_int) {
    _clear();
    type = INT; i_val = p_int; return *this;
}

Variant& Variant::operator=(int p_int) {
    _clear();
    type = INT; i_val = p_int; return *this;
}

Variant& Variant::operator=(const char* p_string) {
    _clear();
    type = STRING; s_val = p_string; return *this;
}

Variant& Variant::operator=(const PackedByteArray& p_packed_byte_array) {
    _clear();
    type = PACKED_BYTE_ARRAY;
    packed_byte_array_val = new PackedByteArray(p_packed_byte_array);
    return *this;
}

bool ProjectSettings::load_resource_pack(const String& p_pack, bool p_replace_files, int p_offset) {
    UtilityFunctions::print("ProjectSettings: Loading resource pack: " + p_pack);
    PCKReader* reader = new PCKReader();
    if (reader->load_pck(p_pack)) {
        Threads::ScopedLock lock(packs_mutex);
        loaded_packs.push_back(reader);
        return true;
    }
    delete reader;
    return false;
}

bool ProjectSettings::is_pack_loaded(const String& p_pack) const {
    Threads::ScopedLock lock(packs_mutex);
    for (size_t i = 0; i < loaded_packs.size(); i++) {
        if (loaded_packs[i]->get_pck_path() == p_pack) return true;
    }
    return false;
}

PCKFileEntry ProjectSettings::find_file_in_packs(const String& p_path, String& r_pck_path) const {
    Threads::ScopedLock lock(packs_mutex);
    for (int i = (int)loaded_packs.size() - 1; i >= 0; i--) {
        if (loaded_packs[i]->file_exists(p_path)) {
            r_pck_path = loaded_packs[i]->get_pck_path();
            return loaded_packs[i]->get_file_entry(p_path);
        }
    }
    return {0, 0, 0};
}

PackedStringArray ProjectSettings::get_files_in_packs(const String& p_path) const {
    PackedStringArray res;
    String prefix = p_path;
    if (!prefix.ends_with("/")) prefix += "/";

    Threads::ScopedLock lock(packs_mutex);
    for (size_t i = 0; i < loaded_packs.size(); i++) {
        std::vector<String> all_files = loaded_packs[i]->get_all_files();
        for (size_t j = 0; j < all_files.size(); j++) {
            String file_path = all_files[j];
            if (file_path.begins_with(prefix.c_str())) {
                String sub = file_path.substr(prefix.length());
                if (!sub.is_empty() && sub.find("/") == std::string::npos) {
                    res.append(sub);
                }
            }
        }
    }
    return res;
}

PackedStringArray ProjectSettings::get_directories_in_packs(const String& p_path) const {
    PackedStringArray res;
    String prefix = p_path;
    if (!prefix.ends_with("/")) prefix += "/";

    Threads::ScopedLock lock(packs_mutex);
    for (size_t i = 0; i < loaded_packs.size(); i++) {
        std::vector<String> all_files = loaded_packs[i]->get_all_files();
        for (size_t j = 0; j < all_files.size(); j++) {
            String file_path = all_files[j];
            if (file_path.begins_with(prefix.c_str())) {
                String sub = file_path.substr(prefix.length());
                size_t slash_pos = sub.find("/");
                if (slash_pos != std::string::npos) {
                    String dir_name = sub.substr(0, slash_pos);
                    if (dir_name.is_empty()) continue;
                    bool already_has = false;
                    for (int k=0; k<res.size(); k++) if (res[k] == dir_name) already_has = true;
                    if (!already_has) res.append(dir_name);
                }
            }
        }
    }
    return res;
}

}

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "../stb_truetype.h"

#include "../webp_wrapper.h"

#include "classes/image.hpp"
#include "classes/font_variation.hpp"
#include "../../graphics/hb_video_driver_psgl.hpp"

namespace godot {

Image::~Image() {
    HBVideoDriverPSGL::on_image_destroyed((uintptr_t)this);
}

FontVariation::FontVariation() {
    font_info = nullptr;
}

FontVariation::~FontVariation() {
    if (font_info) {
        free(font_info);
    }
}

void FontVariation::clear_cache() {
    text_cache.clear();
}

void FontVariation::set_data(const PackedByteArray& p_data) {
    data = p_data;
    clear_cache();
    if (font_info) {
        free(font_info);
        font_info = nullptr;
    }
    if (data.size() > 0) {
        const unsigned char* src_ptr = data.data();
        size_t src_size = data.size();

        stbtt_fontinfo* info = (stbtt_fontinfo*)malloc(sizeof(stbtt_fontinfo));
        if (stbtt_InitFont(info, src_ptr, 0)) {
            font_info = info;
        } else {
            // Try searching for TTF/OTF magic (skip Godot resource header)
            bool found = false;
            
            // Check for Godot 4 Compressed Resource magic "RSCC" (52 53 43 43)
            if (src_size > 4 && src_ptr[0] == 0x52 && src_ptr[1] == 0x53 && src_ptr[2] == 0x43 && src_ptr[3] == 0x43) {
                UtilityFunctions::print("STBTT: Resource is compressed (RSCC). STBTT cannot load compressed fonts. Please re-import fonts with 'Compress' disabled.");
            }

            // Search more thoroughly (first 2048 bytes) for font magic
            for (size_t offset = 0; offset < std::min(src_size, (size_t)2048); offset++) {
                // TTF: 00 01 00 00
                if (offset + 4 <= src_size && src_ptr[offset] == 0x00 && src_ptr[offset+1] == 0x01 && src_ptr[offset+2] == 0x00 && src_ptr[offset+3] == 0x00) {
                    if (stbtt_InitFont(info, src_ptr + offset, 0)) {
                        src_ptr += offset;
                        src_size -= offset;
                        font_info = info;
                        found = true;
                        break;
                    }
                }
                // OTF: OTTO
                if (offset + 4 <= src_size && src_ptr[offset] == 'O' && src_ptr[offset+1] == 'T' && src_ptr[offset+2] == 'T' && src_ptr[offset+3] == 'O') {
                    if (stbtt_InitFont(info, src_ptr + offset, 0)) {
                        src_ptr += offset;
                        src_size -= offset;
                        font_info = info;
                        found = true;
                        break;
                    }
                }
                // WOFF: wOFF
                if (offset + 4 <= src_size && src_ptr[offset] == 'w' && src_ptr[offset+1] == 'O' && src_ptr[offset+2] == 'F' && src_ptr[offset+3] == 'F') {
                    UtilityFunctions::print("STBTT: Found WOFF magic. STBTT does not support WOFF. Use TTF or OTF.");
                }
            }

            if (!found) {
                free(info);
                UtilityFunctions::print("STBTT: Failed to initialize font.");
                if (src_size > 0) {
                    std::string hex_dump = "Head: ";
                    for (size_t i=0; i<std::min(src_size, (size_t)64); i++) {
                        char b[4];
                        sprintf(b, "%02X ", src_ptr[i]);
                        hex_dump += b;
                    }
                    UtilityFunctions::print(hex_dump.c_str());
                }
            }
        }

        if (font_info) {
            char diag[128];
            sprintf(diag, "STBTT: Font initialized successfully. Data size: %zu", src_size);
            UtilityFunctions::print(diag);
        }
    }
}

static uint32_t decode_utf8(const char** s) {
    uint32_t c = (unsigned char)**s;
    if (c == 0) return 0;
    if (c < 0x80) {
        (*s)++;
        return c;
    }
    if ((c & 0xE0) == 0xC0) {
        if ((*s)[1] == 0) { (*s)++; return 0; }
        c = ((c & 0x1F) << 6) | ((unsigned char)(*s)[1] & 0x3F);
        *s += 2;
        return c;
    }
    if ((c & 0xF0) == 0xE0) {
        if ((*s)[1] == 0 || (*s)[2] == 0) { (*s)++; return 0; }
        c = ((c & 0x0F) << 12) | (((unsigned char)(*s)[1] & 0x3F) << 6) | ((unsigned char)(*s)[2] & 0x3F);
        *s += 3;
        return c;
    }
    if ((c & 0xF8) == 0xF0) {
        if ((*s)[1] == 0 || (*s)[2] == 0 || (*s)[3] == 0) { (*s)++; return 0; }
        c = ((c & 0x07) << 18) | (((unsigned char)(*s)[1] & 0x3F) << 12) | (((unsigned char)(*s)[2] & 0x3F) << 6) | ((unsigned char)(*s)[3] & 0x3F);
        *s += 4;
        return c;
    }
    (*s)++;
    return 0;
}

Ref<Image> FontVariation::render_text(const String& p_text, int p_font_size) const {
    if (!font_info || p_text.is_empty()) return Ref<Image>();

    CacheKey key = {p_text, p_font_size};
    if (text_cache.count(key)) {
        // UtilityFunctions::print("STBTT: Cache hit for: " + p_text);
        return text_cache[key];
    }

    UtilityFunctions::print("STBTT: Rendering text: " + p_text);

    stbtt_fontinfo* info = (stbtt_fontinfo*)font_info;

    float scale = stbtt_ScaleForPixelHeight(info, (float)p_font_size);
    
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(info, &ascent, &descent, &lineGap);
    
    std::vector<uint32_t> codepoints;
    const char* p = p_text.c_str();
    while (*p) {
        codepoints.push_back(decode_utf8(&p));
    }

    int total_w = 0;
    for (size_t i = 0; i < codepoints.size(); i++) {
        int ax;
        int lsb;
        stbtt_GetCodepointHMetrics(info, codepoints[i], &ax, &lsb);
        total_w += (int)(ax * scale);
        
        if (i < codepoints.size() - 1) {
            total_w += (int)(stbtt_GetCodepointKernAdvance(info, codepoints[i], codepoints[i+1]) * scale);
        }
    }

    int total_h = (int)((ascent - descent) * scale);
    if (total_w <= 0 || total_h <= 0) return Ref<Image>();

    Ref<Image> img = Image::create(total_w, total_h, false, 3); // Format 3 is RGBA8 in our shim
    PackedByteArray& img_data = img->get_data_rw();
    unsigned char* pixels = img_data.data_ptr();
    memset(pixels, 0, img_data.size());

    int x = 0;
    int baseline = (int)(ascent * scale);

    for (size_t i = 0; i < codepoints.size(); i++) {
        int out_w, out_h, out_xoff, out_yoff;
        unsigned char* bitmap = stbtt_GetCodepointBitmap(info, 0, scale, codepoints[i], &out_w, &out_h, &out_xoff, &out_yoff);
        
        if (bitmap) {
            for (int by = 0; by < out_h; by++) {
                for (int bx = 0; bx < out_w; bx++) {
                    int px = x + out_xoff + bx;
                    int py = baseline + out_yoff + by;
                    if (px >= 0 && px < total_w && py >= 0 && py < total_h) {
                        unsigned char alpha = bitmap[bx + by * out_w];
                        size_t off = (py * total_w + px) * 4;
                        pixels[off + 0] = 255;
                        pixels[off + 1] = 255;
                        pixels[off + 2] = 255;
                        pixels[off + 3] = alpha;
                    }
                }
            }
            stbtt_FreeBitmap(bitmap, nullptr);
        }

        int ax, lsb;
        stbtt_GetCodepointHMetrics(info, codepoints[i], &ax, &lsb);
        x += (int)(ax * scale);
        if (i + 1 < codepoints.size()) {
            x += (int)(stbtt_GetCodepointKernAdvance(info, codepoints[i], codepoints[i+1]) * scale);
        }
    }
    text_cache[key] = img;
    return img;
}


Ref<Image> Image::load_from_buffer(const PackedByteArray& p_buffer) {
    if (p_buffer.size() == 0) return Ref<Image>();

    const unsigned char* src_ptr = p_buffer.data();
    size_t src_size = p_buffer.size();

    // Handle Godot 4 .ctex header (CompressedTexture2D / StreamTexture2D)
    // Magic for StreamTexture2D is "GST2" (47 53 54 32)
    if (src_size > 32 && src_ptr[0] == 'G' && src_ptr[1] == 'S' && src_ptr[2] == 'T' && src_ptr[3] == '2') {
        // Godot 4 texture header is 32 bytes
        // Magic (4), Version (4), Width (4), Height (4), Flags (4), Format (4), Mipmaps (4), Reserved (4)
        
        // Search for image signatures within the first 128 bytes
        size_t limit = std::min(src_size - 12, (size_t)128);
        for (size_t offset = 32; offset <= limit; offset++) {
            // PNG signature: 89 50 4E 47
            if (src_ptr[offset] == 0x89 && src_ptr[offset+1] == 0x50 && src_ptr[offset+2] == 0x4E && src_ptr[offset+3] == 0x47) {
                src_ptr += offset;
                src_size -= offset;
                break;
            }
            // WebP signature: RIFF .... WEBP
            if (src_ptr[offset] == 'R' && src_ptr[offset+1] == 'I' && src_ptr[offset+2] == 'F' && src_ptr[offset+3] == 'F' &&
                src_ptr[offset+8] == 'W' && src_ptr[offset+9] == 'E' && src_ptr[offset+10] == 'B' && src_ptr[offset+11] == 'P') {
                src_ptr += offset;
                src_size -= offset;
                break;
            }
            // JPEG signature: FF D8 FF
            if (src_ptr[offset] == 0xFF && src_ptr[offset+1] == 0xD8 && src_ptr[offset+2] == 0xFF) {
                src_ptr += offset;
                src_size -= offset;
                break;
            }
        }
        // If no signature found, it might be VRAM compressed, which STBI can't handle.
        // But for lossless imports, it should be at offset 32.
    }

    // Try WebP first if it looks like one
    if (src_size > 12 && src_ptr[0] == 'R' && src_ptr[1] == 'I' && src_ptr[2] == 'F' && src_ptr[3] == 'F' &&
        src_ptr[8] == 'W' && src_ptr[9] == 'E' && src_ptr[10] == 'B' && src_ptr[11] == 'P') {
        
        jebp_image_t jebp_img;
        jebp_error_t err = jebp_decode(&jebp_img, src_size, src_ptr);
        if (err == JEBP_OK) {
            Ref<Image> img = Image::create(jebp_img.width, jebp_img.height, false, 0);
            if (img.is_valid()) {
                PackedByteArray img_data;
                img_data.resize(jebp_img.width * jebp_img.height * 4);
                if (img_data.size() > 0) {
                    memcpy(img_data.data_ptr(), jebp_img.pixels, jebp_img.width * jebp_img.height * 4);
                    img->set_data(img_data);
                }
            }
            jebp_free_image(&jebp_img);
            return img;
        } else {
            char diag[256];
            sprintf(diag, "JEBP load failed: %s", jebp_error_string(err));
            UtilityFunctions::print(diag);
            // Fallthrough to STBI just in case
        }
    }

    int w, h, channels;
    unsigned char* data_ptr = stbi_load_from_memory(src_ptr, src_size, &w, &h, &channels, 4);
    if (!data_ptr) {
        char diag[256];
        sprintf(diag, "STBI load failed: %s (Buffer size: %zu)", stbi_failure_reason(), src_size);
        UtilityFunctions::print(diag);
        if (src_size > 0) {
            std::string hex_dump = "Head: ";
            for (size_t i=0; i<std::min(src_size, (size_t)64); i++) {
                char b[4];
                sprintf(b, "%02X ", src_ptr[i]);
                hex_dump += b;
            }
            UtilityFunctions::print(hex_dump.c_str());
        }
        return Ref<Image>();
    }

    Ref<Image> img = Image::create(w, h, false, 0);
    PackedByteArray img_data;
    img_data.resize(w * h * 4);
    memcpy(img_data.data_ptr(), data_ptr, w * h * 4);
    img->set_data(img_data);

    stbi_image_free(data_ptr);
    return img;
}

}
