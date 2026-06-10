#ifndef HB_ERROR_HPP
#define HB_ERROR_HPP

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/variant.hpp>

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
