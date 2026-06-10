#include "hb_web_user_info.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBWebUserInfoNative::_bind_methods() {}

HBWebUserInfoNative::HBWebUserInfoNative() {
    serializable_fields.append("id");
    serializable_fields.append("steam_id");
    serializable_fields.append("is_admin");
    serializable_fields.append("level");
    serializable_fields.append("experience");
    serializable_fields.append("rating");
}

HBWebUserInfoNative::~HBWebUserInfoNative() {}
