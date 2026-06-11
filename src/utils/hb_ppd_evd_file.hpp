#ifndef HB_PPD_EVD_FILE_HPP
#define HB_PPD_EVD_FILE_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/array.hpp>

namespace godot {

class PPDEVDFileNative : public RefCounted {
    GDCLASS(PPDEVDFileNative, RefCounted);

protected:
    static void _bind_methods();

private:
    Array evd_events;

public:
    enum PPDEventType {
        ChangeVolume = 0,
        ChangeBPM = 1,
        RapidChangeBPM = 2,
        ChangeSoundPlayMode = 3,
        ChangeDisplayState = 4,
        ChangeMoveState = 5,
        ChangeReleaseSound = 6,
        ChangeNoteType = 7,
        ChangeInitializeOrder = 8,
        ChangeSlideScale = 9
    };

    PPDEVDFileNative();
    ~PPDEVDFileNative();

    int get_note_type_at_time(double p_time);
    double get_slide_scale_at_time(double p_time);

    VARIANT_ENUM_CAST(PPDEventType);
};

} // namespace godot

#endif // HB_PPD_EVD_FILE_HPP
