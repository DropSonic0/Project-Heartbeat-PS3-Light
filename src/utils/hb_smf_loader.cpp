#include "hb_smf_loader.hpp"
#ifdef __PPU__
#include "compat/godot_cpp/core/class_db.hpp"
#include "compat/godot_cpp/classes/file_access.hpp"
#else
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/file_access.hpp>
#endif

using namespace godot;

void SMFLoaderNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("read_file", "path"), &SMFLoaderNative::read_file);
}

SMFLoaderNative::SMFLoaderNative() {}
SMFLoaderNative::~SMFLoaderNative() {}

static uint32_t _read_variable_int(Ref<FileAccess> f) {
    uint32_t result = 0;
    while (true) {
        uint8_t c = f->get_8();
        result = (result << 7) | (c & 0x7f);
        if (!(c & 0x80)) {
            break;
        }
    }
    return result;
}

Dictionary SMFLoaderNative::read_file(const String &p_path) {
    Dictionary res;
    Ref<FileAccess> f = FileAccess::open(p_path, FileAccess::READ);
    if (f.is_null()) return res;

    // Header Chunk
    char chunk_id[5] = {0};
    PackedByteArray id_buf = f->get_buffer(4);
    for(int i=0; i<4; i++) chunk_id[i] = (char)id_buf[i];
    
    if (String(chunk_id) != "MThd") return res;

    uint32_t header_size = f->get_32();
    res["format_type"] = (int)f->get_16();
    int track_count = (int)f->get_16();
    res["track_count"] = track_count;
    res["timebase"] = (int)f->get_16();

    Array tracks;
    for (int i=0; i<track_count; i++) {
        Dictionary track;
        id_buf = f->get_buffer(4);
        for(int j=0; j<4; j++) chunk_id[j] = (char)id_buf[j];
        if (String(chunk_id) != "MTrk") break;

        uint32_t track_size = f->get_32();
        size_t track_end = f->get_position() + track_size;

        Array events;
        uint32_t time = 0;
        uint8_t last_status = 0;

        while (f->get_position() < track_end && !f->eof_reached()) {
            uint32_t delta = _read_variable_int(f);
            time += delta;
            uint8_t status = f->get_8();

            if (status < 0x80) {
                // Running status
                f->seek(f->get_position() - 1);
                status = last_status;
            } else {
                last_status = status;
            }

            Dictionary ev;
            ev["time"] = (int64_t)time;
            ev["status"] = (int)status;
            
            uint8_t type = status & 0xF0;
            Dictionary args;
            if (type == 0x80 || type == 0x90 || type == 0xA0 || type == 0xB0 || type == 0xE0) {
                args["p1"] = (int)f->get_8();
                args["p2"] = (int)f->get_8();
            } else if (type == 0xC0 || type == 0xD0) {
                args["p1"] = (int)f->get_8();
            } else if (status == 0xFF) {
                uint8_t meta_type = f->get_8();
                args["type"] = (int)meta_type;
                uint32_t meta_len = _read_variable_int(f);
                PackedByteArray meta_data = f->get_buffer(meta_len);
                args["data"] = (Variant)meta_data;
                if (meta_type <= 0x07) {
                    String text = "";
                    for (int k=0; k<(int)meta_data.size(); k++) text += (char)meta_data[k];
                    args["text"] = text;
                }
            }
            ev["args"] = args;
            events.append(ev);
        }
        track["events"] = events;
        tracks.append(track);
    }
    res["tracks"] = tracks;

    return res;
}
