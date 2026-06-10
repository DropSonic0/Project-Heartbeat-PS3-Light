#ifndef HB_CHART_HPP
#define HB_CHART_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace godot {

class HBChartNative : public RefCounted {
    GDCLASS(HBChartNative, RefCounted);

protected:
    static void _bind_methods();

private:
    Array layers;
    int format_version = 2;

public:
    HBChartNative();
    ~HBChartNative();

    void populate_layers();
    Array get_timing_points(bool p_ordered = true);
    Dictionary serialize();
    void deserialize(Dictionary p_data, Variant p_song);
    double get_max_score();
    static Dictionary get_slide_hold_chains_for_points(Array p_timing_points);
    Dictionary get_slide_hold_chains();
    int get_layer_i(String p_layer_name);

    bool _note_comparison_internal(const Variant &a, const Variant &b);
};

} // namespace godot

#endif // HB_CHART_HPP
