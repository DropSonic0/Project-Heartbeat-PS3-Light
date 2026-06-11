#ifndef HB_GAME_INFO_HPP
#define HB_GAME_INFO_HPP

#include "hb_serializable.hpp"
#include "hb_result.hpp"

namespace godot {

class HBGameInfoNative : public HBSerializableNative {
    GDCLASS(HBGameInfoNative, HBSerializableNative);

protected:
    static void _bind_methods();

public:
    enum GameSessionType {
        OFFLINE,
        MULTIPLAYER
    };

private:
    GameSessionType game_session_type = OFFLINE;
    int64_t time = 0;
    Ref<HBResultNative> result;
    Dictionary modifiers;
    bool used_autoplay = false;
    String song_id;
    String difficulty;
    int32_t variant = -1;

public:
    HBGameInfoNative();
    ~HBGameInfoNative();

    void set_game_session_type(GameSessionType p_type) { game_session_type = p_type; }
    GameSessionType get_game_session_type() const { return game_session_type; }
    void set_time(int64_t p_time) { time = p_time; }
    int64_t get_time() const { return time; }
    void set_result(Ref<HBResultNative> p_result) { result = p_result; }
    Ref<HBResultNative> get_result() const { return result; }
    void set_modifiers(Dictionary p_modifiers) { modifiers = p_modifiers; }
    Dictionary get_modifiers() const { return modifiers; }
    void set_song_id(String p_id) { song_id = p_id; }
    String get_song_id() const { return song_id; }
    void set_difficulty(String p_diff) { difficulty = p_diff; }
    String get_difficulty() const { return difficulty; }

    bool is_leaderboard_legal() const;

    virtual String get_serialized_type() const GD_OVERRIDE { return "GameInfo"; }

    VARIANT_ENUM_CAST(GameSessionType);
};

} // namespace godot

#endif // HB_GAME_INFO_HPP
