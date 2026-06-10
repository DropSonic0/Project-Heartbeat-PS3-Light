#ifndef HB_UI_COMPONENT_HPP
#define HB_UI_COMPONENT_HPP

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/color.hpp>

namespace godot {

class HBUIComponentNative : public Control {
    GDCLASS(HBUIComponentNative, Control);

protected:
    static void _bind_methods();

public:
    HBUIComponentNative();
    ~HBUIComponentNative();

    virtual String get_component_id() const { return "component"; }
    virtual String get_component_name() const { return "HBComponent"; }

    String get_color_string(Color p_color) const;
    Color get_color_from_dict(Dictionary p_dict, String p_key, Color p_backup) const;

    void _ready() GD_OVERRIDE;
};

} // namespace godot

#endif // HB_UI_COMPONENT_HPP
