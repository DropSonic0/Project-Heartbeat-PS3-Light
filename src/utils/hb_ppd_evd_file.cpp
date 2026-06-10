#include "hb_ppd_evd_file.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void PPDEVDFileNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_note_type_at_time", "time"), &PPDEVDFileNative::get_note_type_at_time);
    ClassDB::bind_method(D_METHOD("get_slide_scale_at_time", "time"), &PPDEVDFileNative::get_slide_scale_at_time);

    BIND_ENUM_CONSTANT(ChangeVolume);
    BIND_ENUM_CONSTANT(ChangeBPM);
    BIND_ENUM_CONSTANT(RapidChangeBPM);
    BIND_ENUM_CONSTANT(ChangeSoundPlayMode);
    BIND_ENUM_CONSTANT(ChangeDisplayState);
    BIND_ENUM_CONSTANT(ChangeMoveState);
    BIND_ENUM_CONSTANT(ChangeReleaseSound);
    BIND_ENUM_CONSTANT(ChangeNoteType);
    BIND_ENUM_CONSTANT(ChangeInitializeOrder);
    BIND_ENUM_CONSTANT(ChangeSlideScale);
}

PPDEVDFileNative::PPDEVDFileNative() {}
PPDEVDFileNative::~PPDEVDFileNative() {}

int PPDEVDFileNative::get_note_type_at_time(double p_time) {
    int note_type = 0;
    for (int i = 0; i < evd_events.size(); i++) {
        Dictionary event = evd_events[i];
        if ((int)event["event_type"] == ChangeNoteType) {
            if ((double)event["time"] > p_time) break;
            note_type = event["note_type"];
        }
    }
    return note_type;
}

double PPDEVDFileNative::get_slide_scale_at_time(double p_time) {
    double slide_scale = 1.0;
    for (int i = 0; i < evd_events.size(); i++) {
        Dictionary event = evd_events[i];
        if ((int)event["event_type"] == ChangeSlideScale) {
            if ((double)event["time"] > p_time) break;
            slide_scale = event["slide_scale"];
        }
    }
    return slide_scale;
}
