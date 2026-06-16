#ifndef HB_HISTORY_ENTRY_HPP
#define HB_HISTORY_ENTRY_HPP

#include "hb_serializable.hpp"
#include "hb_game_info.hpp"

namespace godot {

class HBHistoryEntryNative : public HBSerializableNative {
    GDCLASS(HBHistoryEntryNative, HBSerializableNative);

protected:
    static void _bind_methods();

private:
    Ref<HBGameInfoNative> highest_score_info;
    int64_t highest_score = 0;
    int32_t highest_rating = 0;
    double highest_percentage = 0.0;

public:
    HBHistoryEntryNative();
    ~HBHistoryEntryNative();

    bool is_result_better(Ref<HBResultNative> p_result);
    void update_entry(Ref<HBGameInfoNative> p_game_info);

    virtual String get_serialized_type() const GD_OVERRIDE { return "HBHistoryEntry"; }
};

} // namespace godot

#endif // HB_HISTORY_ENTRY_HPP
