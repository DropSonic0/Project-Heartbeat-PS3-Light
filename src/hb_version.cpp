#include "hb_version.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp>

using namespace godot;

void HBVersionNative::_bind_methods() {
    ClassDB::bind_static_method("HBVersionNative", D_METHOD("get_version_string", "with_line_breaks"), &HBVersionNative::get_version_string, DEFVAL(false));
}

String HBVersionNative::get_version_string(bool p_with_line_breaks) {
    String version = String::num_int64(MAJOR) + "." + String::num_int64(MINOR) + "." + String::num_int64(PATCH);
    String result = "Project Heartbeat: Marina's Legacy - Release Candidate 5 - " + version;
    if (p_with_line_breaks) {
        result = result.replace(" - ", "\n");
    }
    return result;
}
