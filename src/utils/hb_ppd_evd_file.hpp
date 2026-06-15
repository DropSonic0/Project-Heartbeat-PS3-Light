#ifndef HB_PPD_EVD_FILE_HPP
#define HB_PPD_EVD_FILE_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/ref_counted.hpp"
#include "compat/godot_cpp/classes/file_access.hpp"
#include "compat/godot_cpp/variant/array.hpp"
#include "compat/godot_cpp/variant/dictionary.hpp"
#else
#ifdef __PPU__
#include "compat/godot_cpp/classes/ref_counted.hpp"
#else
#include <godot_cpp/classes/ref_counted.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/classes/file_access.hpp"
#else
#include <godot_cpp/classes/file_access.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/variant/array.hpp"
#else
#include <godot_cpp/variant/array.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/variant/dictionary.hpp"
#else
#include <godot_cpp/variant/dictionary.hpp>
#endif
#endif

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

    void load_from_file(Ref<FileAccess> p_file, size_t p_file_length, size_t p_file_offset);
    int get_note_type_at_time(double p_time);
    double get_slide_scale_at_time(double p_time);

    VARIANT_ENUM_CAST(PPDEventType);
};

} // namespace godot

#endif // HB_PPD_EVD_FILE_HPP
