#ifndef HB_ERROR_HPP
#define HB_ERROR_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/object.hpp"
#else
#include <godot_cpp/classes/object.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/variant/string.hpp"
#else
#include <godot_cpp/variant/string.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/variant/variant.hpp"
#else
#include <godot_cpp/variant/variant.hpp>
#endif

namespace godot {

class HBErrorNative : public Object {
    GDCLASS(HBErrorNative, Object);

protected:
    static void _bind_methods();

private:
    int32_t error_type;
    String error_message;
    Variant caller_data;

public:
    enum OwnErrors {
        CUSTOM_ERROR = 49 // ERR_PRINTER_ON_FIRE + 1
    };

    HBErrorNative();
    ~HBErrorNative();

    void set_error_type(int32_t p_type) { error_type = p_type; }
    int32_t get_error_type() const { return error_type; }
    void set_error_message(String p_msg) { error_message = p_msg; }
    String get_error_message() const { return error_message; }

    String to_string() const;
};

} // namespace godot

#endif // HB_ERROR_HPP
