#ifndef HB_BPM_CHANGE_HPP
#define HB_BPM_CHANGE_HPP

#include "hb_timing_point.hpp"

namespace godot {

class HBBPMChangeNative : public HBTimingPointNative {
    GDCLASS(HBBPMChangeNative, HBTimingPointNative);

protected:
    static void _bind_methods();

public:
    enum UsageTypes {
        FIXED_BPM,
        AUTO_BPM
    };

private:
    double bpm = 180.0;
    double speed_factor = 100.0;
    UsageTypes usage = AUTO_BPM;

public:
    HBBPMChangeNative();
    ~HBBPMChangeNative();

    void set_bpm(double p_bpm);
    double get_bpm() const;

    void set_speed_factor(double p_speed_factor);
    double get_speed_factor() const;

    void set_usage(UsageTypes p_usage);
    UsageTypes get_usage() const;

    virtual String get_serialized_type() const GD_OVERRIDE;
};

} // namespace godot

VARIANT_ENUM_CAST(HBBPMChangeNative::UsageTypes);

#endif // HB_BPM_CHANGE_HPP
