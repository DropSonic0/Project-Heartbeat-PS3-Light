#ifndef HB_INTRO_SKIP_MARKER_HPP
#define HB_INTRO_SKIP_MARKER_HPP

#include "hb_timing_point.hpp"

namespace godot {

class HBIntroSkipMarkerNative : public HBTimingPointNative {
    GDCLASS(HBIntroSkipMarkerNative, HBTimingPointNative);

protected:
    static void _bind_methods();

public:
    HBIntroSkipMarkerNative();
    ~HBIntroSkipMarkerNative();

    virtual String get_serialized_type() const override;
};

} // namespace godot

#endif // HB_INTRO_SKIP_MARKER_HPP
