#ifndef HB_TIMING_POINT_HPP
#define HB_TIMING_POINT_HPP

#include "hb_serializable.hpp"

#include <stdint.h>

namespace godot {

class HBTimingPointNative : public HBSerializableNative {
    GDCLASS(HBTimingPointNative, HBSerializableNative);

protected:
    static void _bind_methods();

private:
    int64_t time = 0;

public:
    HBTimingPointNative();
    ~HBTimingPointNative();

    void set_time(int64_t p_time);
    int64_t get_time() const;

    virtual String get_serialized_type() const GD_OVERRIDE;
};

} // namespace godot

#endif // HB_TIMING_POINT_HPP
