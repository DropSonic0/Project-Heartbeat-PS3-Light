#ifndef HB_CHART_SECTION_HPP
#define HB_CHART_SECTION_HPP

#include "hb_timing_point.hpp"
#include <godot_cpp/variant/color.hpp>

namespace godot {

class HBChartSectionNative : public HBTimingPointNative {
    GDCLASS(HBChartSectionNative, HBTimingPointNative);

protected:
    static void _bind_methods();

private:
    String name = "New section";
    Color color;

public:
    HBChartSectionNative();
    ~HBChartSectionNative();

    void set_section_name(String p_name);
    String get_section_name() const;

    void set_color(Color p_color);
    Color get_color() const;

    virtual String get_serialized_type() const GD_OVERRIDE;
};

} // namespace godot

#endif // HB_CHART_SECTION_HPP
