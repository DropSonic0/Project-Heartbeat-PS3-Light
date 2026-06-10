#ifndef HB_PPD_PACK_HPP
#define HB_PPD_PACK_HPP

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/file_access.hpp>

namespace godot {

class PPDPackNative : public Node {
    GDCLASS(PPDPackNative, Node);

protected:
    static void _bind_methods();

private:
    Ref<FileAccess> file;
    bool valid = true;

public:
    PPDPackNative();
    ~PPDPackNative();

    int get_file_index(const String &p_file_name);
};

} // namespace godot

#endif // HB_PPD_PACK_HPP
