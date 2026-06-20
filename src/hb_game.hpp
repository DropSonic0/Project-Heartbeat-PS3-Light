#ifndef HB_GAME_HPP
#define HB_GAME_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/node.hpp"
#else
#include <godot_cpp/classes/node.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/variant/dictionary.hpp"
#else
#include <godot_cpp/variant/dictionary.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/variant/array.hpp"
#else
#include <godot_cpp/variant/array.hpp>
#endif
#include "ui/hb_main_menu_native.hpp"
#include "threads/thread.hpp"
#include "threads/mutex.hpp"
#include "compat/godot_cpp/classes/image.hpp"

namespace godot {

class HBGameNative : public Node {
    GDCLASS(HBGameNative, Node);

protected:
    static void _bind_methods();

private:
    bool demo_mode = false;
    Dictionary serializable_types;
    Node* current_ui = nullptr;
    
    Ref<Image> splash_image;
    bool is_loading = true;
    Threads::Mutex loading_mutex;
    Threads::Thread *loading_thread = nullptr;

    void _background_load();

public:
    HBGameNative();
    ~HBGameNative();

    void set_demo_mode(bool p_demo_mode);
    bool get_demo_mode() const;

    Dictionary get_serializable_types() const;
    void register_serializable_type(const String &p_name, const Variant &p_type);

    void change_to_menu(const String& p_menu, const Variant& p_param = Variant());

    void main_loop_step();

    static HBGameNative *get_singleton();
};

} // namespace godot

#endif // HB_GAME_HPP
