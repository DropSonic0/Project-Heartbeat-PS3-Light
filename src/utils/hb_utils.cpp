#include "hb_utils.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/reg_ex.hpp>
#include <cmath>

using namespace godot;

void HBUtilsNative::_bind_methods() {
    ClassDB::bind_static_method("HBUtilsNative", D_METHOD("format_time", "time", "format", "digit_format"), &HBUtilsNative::format_time, DEFVAL(14), DEFVAL("%02d"));
    ClassDB::bind_static_method("HBUtilsNative", D_METHOD("merge_dict", "target", "patch"), &HBUtilsNative::merge_dict);
    ClassDB::bind_static_method("HBUtilsNative", D_METHOD("get_valid_filename", "value"), &HBUtilsNative::get_valid_filename);
    ClassDB::bind_static_method("HBUtilsNative", D_METHOD("calculate_note_sine", "time", "pos", "angle", "frequency", "amplitude", "distance"), &HBUtilsNative::calculate_note_sine);
    ClassDB::bind_static_method("HBUtilsNative", D_METHOD("thousands_sep", "number", "prefix"), &HBUtilsNative::thousands_sep, DEFVAL(""));
    ClassDB::bind_static_method("HBUtilsNative", D_METHOD("remap", "value", "low1", "low2", "high1", "high2"), &HBUtilsNative::remap);
    ClassDB::bind_static_method("HBUtilsNative", D_METHOD("join_path", "path1", "path2"), &HBUtilsNative::join_path);
    ClassDB::bind_static_method("HBUtilsNative", D_METHOD("get_experience_to_next_level", "level"), &HBUtilsNative::get_experience_to_next_level);
    ClassDB::bind_static_method("HBUtilsNative", D_METHOD("wrap_text", "text", "length"), &HBUtilsNative::wrap_text, DEFVAL(25));
    ClassDB::bind_static_method("HBUtilsNative", D_METHOD("bsearch_upper", "array", "value"), &HBUtilsNative::bsearch_upper);
    ClassDB::bind_static_method("HBUtilsNative", D_METHOD("bsearch_closest", "array", "value"), &HBUtilsNative::bsearch_closest);
    ClassDB::bind_static_method("HBUtilsNative", D_METHOD("bsearch_linear", "array", "value"), &HBUtilsNative::bsearch_linear);

    BIND_ENUM_CONSTANT(FORMAT_HOURS);
    BIND_ENUM_CONSTANT(FORMAT_MINUTES);
    BIND_ENUM_CONSTANT(FORMAT_SECONDS);
    BIND_ENUM_CONSTANT(FORMAT_MILISECONDS);
    BIND_ENUM_CONSTANT(FORMAT_DEFAULT);
}

HBUtilsNative::HBUtilsNative() {}
HBUtilsNative::~HBUtilsNative() {}

String HBUtilsNative::format_time(float p_time, int p_format, const String &p_digit_format) {
    Array digits;
    double s = std::abs(p_time) / 1000.0;

    if (p_format & FORMAT_HOURS) {
        digits.append(String::num_int64((int64_t)(s / 3600)).lpad(2, "0"));
    }

    if (p_format & FORMAT_MINUTES) {
        int64_t minutes = (int64_t)(s / 60);
        if (p_format & FORMAT_HOURS) {
            minutes %= 60;
        }
        digits.append(String::num_int64(minutes).lpad(2, "0"));
    }

    if (p_format & FORMAT_SECONDS) {
        digits.append(String::num_int64((int64_t)std::fmod(s, 60.0)).lpad(2, "0"));
    }

    String formatted = "";
    String colon = ":";

    for (int i = 0; i < digits.size(); i++) {
        formatted += String(digits[i]) + colon;
    }

    if (formatted.length() > 0) {
        formatted = formatted.erase(formatted.length() - 1, 1);
    }

    if (p_format & FORMAT_MILISECONDS) {
        formatted += "." + String::num_int64((int64_t)std::fmod(p_time, 1000.0)).lpad(3, "0");
    }

    if (p_time < 0.0) {
        formatted = "-" + formatted;
    }

    return formatted;
}

Dictionary HBUtilsNative::merge_dict(Dictionary p_target, Dictionary p_patch) {
    Array keys = p_patch.keys();
    for (int i = 0; i < keys.size(); i++) {
        Variant key = keys[i];
        if (p_target.has(key)) {
            Variant tv = p_target[key];
            if (tv.get_type() == Variant::DICTIONARY) {
                merge_dict(tv, p_patch[key]);
            } else {
                p_target[key] = p_patch[key];
            }
        } else {
            p_target[key] = p_patch[key];
        }
    }
    return p_target;
}

String HBUtilsNative::get_valid_filename(const String &p_value) {
    String value = p_value.strip_edges().replace(" ", "_");
    static Ref<RegEx> regex;
    if (regex.is_null()) {
        regex.instantiate();
        regex->compile("[^a-zA-Z0-9_]");
    }
    return regex->sub(value, "", true).to_lower();
}

Vector2 HBUtilsNative::calculate_note_sine(float p_time, Vector2 p_pos, float p_angle, float p_frequency, float p_amplitude, float p_distance) {
    if (p_distance == 0.0f) {
        return p_pos;
    } else if (std::fmod(p_frequency, 2.0f) != 0.0f) {
        p_frequency *= -1.0f;
    }
    p_time = 1.0f - p_time;
    float point_x = p_time * p_distance;
    float point_y = std::sin(p_time * Math_PI * p_frequency) / 12.0f * p_amplitude;
    Vector2 point = Vector2(point_x, point_y).rotated(UtilityFunctions::deg_to_rad(p_angle)) + p_pos;
    return point;
}

String HBUtilsNative::thousands_sep(int64_t p_number, const String &p_prefix) {
    bool neg = false;
    if (p_number < 0) {
        p_number = -p_number;
        neg = true;
    }
    String s = String::num_int64(p_number);
    int mod = s.length() % 3;
    String res = "";
    for (int i = 0; i < s.length(); i++) {
        if (i != 0 && i % 3 == mod) {
            res += ",";
        }
        res += s[i];
    }
    if (neg) {
        res = "-" + p_prefix + res;
    } else {
        res = p_prefix + res;
    }
    return res;
}

float HBUtilsNative::remap(float p_value, float p_low1, float p_low2, float p_high1, float p_high2) {
    return p_low2 + (p_value - p_low1) * (p_high2 - p_low2) / (p_high1 - p_low1);
}

String HBUtilsNative::join_path(const String &p_path1, const String &p_path2) {
    String path1 = p_path1;
    if (!path1.ends_with("/") && !p_path2.begins_with("/")) {
        path1 += "/";
    }
    return path1 + p_path2;
}

int64_t HBUtilsNative::get_experience_to_next_level(int32_t p_level) {
    return 500 + (500 * p_level);
}

String HBUtilsNative::wrap_text(const String &p_text, int32_t p_length) {
    PackedStringArray words = p_text.split(" ");
    Array lines;
    lines.append(String(""));

    for (int i = 0; i < words.size(); i++) {
        String word = words[i];
        String last_line = lines[lines.size() - 1];
        if (last_line.length() + word.length() > p_length) {
            lines.append(word);
        } else {
            if (last_line.length() > 0) {
                lines[lines.size() - 1] = last_line + " " + word;
            } else {
                lines[lines.size() - 1] = word;
            }
        }
    }

    String new_text = "";
    for (int i = 0; i < lines.size(); i++) {
        new_text += String(lines[i]);
        if (i < lines.size() - 1) {
            new_text += "\n";
        }
    }
    return new_text;
}

int64_t HBUtilsNative::bsearch_upper(Array p_array, int64_t p_value) {
    int64_t count = p_array.size() - 1;
    int64_t idx = 0;

    while (count > 0) {
        int64_t step = count / 2;
        if (p_value >= (int64_t)p_array[idx + step]) {
            idx += step + 1;
            count -= step + 1;
        } else {
            count = step;
        }
    }
    return idx;
}

int64_t HBUtilsNative::bsearch_closest(Array p_array, int64_t p_value) {
    if (p_array.is_empty()) {
        return 0;
    }
    int64_t idx = std::min((int64_t)p_array.bsearch(p_value), (int64_t)p_array.size() - 1);
    if (idx == 0 || std::abs((int64_t)p_array[idx] - p_value) < std::abs((int64_t)p_array[idx-1] - p_value)) {
        return idx;
    } else {
        return idx - 1;
    }
}

float HBUtilsNative::bsearch_linear(Array p_array, int64_t p_value) {
    if (p_array.is_empty()) {
        return 0.0f;
    }
    int64_t idx = p_array.bsearch(p_value);

    if (idx > 0 && (idx == p_array.size() || (int64_t)p_array[idx] > p_value)) {
        idx--;
    }

    if (idx + 1 >= p_array.size() || (int64_t)p_array[idx] == p_value || (int64_t)p_array[idx] == (int64_t)p_array[idx+1]) {
        return (float)idx;
    }

    float decimal = UtilityFunctions::inverse_lerp((float)p_array[idx], (float)p_array[idx+1], (float)p_value);
    return (float)idx + decimal;
}
