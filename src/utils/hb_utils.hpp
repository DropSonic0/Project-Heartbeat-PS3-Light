#ifndef HB_UTILS_HPP
#define HB_UTILS_HPP

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/texture2d.hpp>

namespace godot {

class HBUtilsNative : public Object {
    GDCLASS(HBUtilsNative, Object);

protected:
    static void _bind_methods();

public:
    enum TimeFormat {
        FORMAT_HOURS = 1,
        FORMAT_MINUTES = 2,
        FORMAT_SECONDS = 4,
        FORMAT_MILISECONDS = 8,
        FORMAT_DEFAULT = 14
    };

    HBUtilsNative();
    ~HBUtilsNative();

    static String format_time(float p_time, int p_format = FORMAT_DEFAULT, const String &p_digit_format = "%02d");
    static Dictionary merge_dict(Dictionary p_target, Dictionary p_patch);
    static String get_valid_filename(const String &p_value);
    static Vector2 calculate_note_sine(float p_time, Vector2 p_pos, float p_angle, float p_frequency, float p_amplitude, float p_distance);
    static String thousands_sep(int64_t p_number, const String &p_prefix = "");
    static float remap(float p_value, float p_low1, float p_low2, float p_high1, float p_high2);
    static String join_path(const String &p_path1, const String &p_path2);
    static int64_t get_experience_to_next_level(int32_t p_level);
    static String wrap_text(const String &p_text, int32_t p_length = 25);
    static int64_t bsearch_upper(Array p_array, int64_t p_value);
    static int64_t bsearch_closest(Array p_array, int64_t p_value);
    static float bsearch_linear(Array p_array, int64_t p_value);

    VARIANT_ENUM_CAST(TimeFormat);
};

} // namespace godot

#endif // HB_UTILS_HPP
