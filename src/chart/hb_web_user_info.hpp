#ifndef HB_WEB_USER_INFO_HPP
#define HB_WEB_USER_INFO_HPP

#include "hb_serializable.hpp"

namespace godot {

class HBWebUserInfoNative : public HBSerializableNative {
    GDCLASS(HBWebUserInfoNative, HBSerializableNative);

protected:
    static void _bind_methods();

private:
    int64_t id = 0;
    String steam_id;
    bool is_admin = false;
    int32_t level = 1;
    int64_t experience = 0;
    double rating = 0.0;

public:
    HBWebUserInfoNative();
    ~HBWebUserInfoNative();

    virtual String get_serialized_type() const GD_OVERRIDE { return "WebUserInfo"; }
};

} // namespace godot

#endif // HB_WEB_USER_INFO_HPP
