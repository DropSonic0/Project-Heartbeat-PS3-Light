#include "hb_ppd_evd_file.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void PPDEVDFileNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("load_from_file", "file", "file_length", "file_offset"), &PPDEVDFileNative::load_from_file);
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

void PPDEVDFileNative::load_from_file(Ref<FileAccess> p_file, size_t p_file_length, size_t p_file_offset) {
    if (p_file.is_null()) return;
    evd_events.clear();
    p_file->seek(p_file_offset);
    while (p_file->get_position() < p_file_offset + p_file_length && !p_file->eof_reached()) {
        float time = p_file->get_float();

        uint8_t mode = p_file->get_8();
        Dictionary event;
        event["time"] = (double)time;
        event["event_type"] = (int)mode;

        switch (mode) {
            case ChangeVolume: {
                p_file->get_8(); // channel
                p_file->get_8(); // volpercent
            } break;
            case ChangeBPM: {
                event["target_bpm"] = (double)p_file->get_float();
            } break;
            case RapidChangeBPM: {
                event["target_bpm"] = (double)p_file->get_float();
                p_file->get_8(); // rapid
            } break;
            case ChangeSoundPlayMode: {
                p_file->get_8(); // channel
                p_file->get_8(); // keep_playing
            } break;
            case ChangeDisplayState: {
                p_file->get_8(); // dstate
            } break;
            case ChangeMoveState: {
                p_file->get_8(); // mstate
            } break;
            case ChangeReleaseSound: {
                p_file->get_8(); // channel
                p_file->get_8(); // release_sound
            } break;
            case ChangeNoteType: {
                event["note_type"] = (int)p_file->get_8();
            } break;
            case ChangeInitializeOrder: {
                p_file->get_buffer(10);
            } break;
            case ChangeSlideScale: {
                event["slide_scale"] = (double)p_file->get_float();
            } break;
        }
        evd_events.append(event);
    }
}

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
