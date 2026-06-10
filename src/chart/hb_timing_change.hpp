#ifndef HB_TIMING_CHANGE_HPP
#define HB_TIMING_CHANGE_HPP

#include "hb_timing_point.hpp"

namespace godot {

class HBTimingChangeNative : public HBTimingPointNative {
    GDCLASS(HBTimingChangeNative, HBTimingPointNative);

protected:
    static void _bind_methods();

private:
    double bpm = 120.0;
    Dictionary time_signature;

public:
    HBTimingChangeNative();
    ~HBTimingChangeNative();

    void set_bpm(double p_bpm);
    double get_bpm() const;

    void set_time_signature(Dictionary p_time_signature);
    Dictionary get_time_signature() const;

    virtual String get_serialized_type() const override;
};

} // namespace godot

#endif // HB_TIMING_CHANGE_HPP
