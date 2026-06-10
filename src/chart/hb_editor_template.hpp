#ifndef HB_EDITOR_TEMPLATE_HPP
#define HB_EDITOR_TEMPLATE_HPP

#include "hb_serializable.hpp"
#include "hb_base_note.hpp"

namespace godot {

class HBEditorTemplateNative : public HBSerializableNative {
    GDCLASS(HBEditorTemplateNative, HBSerializableNative);

protected:
    static void _bind_methods();

private:
    String name = "New Template";
    Array saved_properties;
    bool autohide = false;

    Ref<HBBaseNoteNative> up_template;
    Ref<HBBaseNoteNative> down_template;
    Ref<HBBaseNoteNative> left_template;
    Ref<HBBaseNoteNative> right_template;
    Ref<HBBaseNoteNative> slide_left_template;
    Ref<HBBaseNoteNative> slide_right_template;
    Ref<HBBaseNoteNative> slide_chain_left_template;
    Ref<HBBaseNoteNative> slide_chain_right_template;
    Ref<HBBaseNoteNative> slide_left_2_template;
    Ref<HBBaseNoteNative> slide_right_2_template;
    Ref<HBBaseNoteNative> slide_chain_left_2_template;
    Ref<HBBaseNoteNative> slide_chain_right_2_template;
    Ref<HBBaseNoteNative> heart_template;

public:
    HBEditorTemplateNative();
    ~HBEditorTemplateNative();

    virtual String get_serialized_type() const override { return "EditorTemplate"; }
};

} // namespace godot

#endif // HB_EDITOR_TEMPLATE_HPP
