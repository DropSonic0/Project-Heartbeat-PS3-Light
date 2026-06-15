#ifndef HB_PPD_PACK_HPP
#define HB_PPD_PACK_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/node.hpp"
#else
#include <godot_cpp/classes/node.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/classes/file_access.hpp"
#else
#include <godot_cpp/classes/file_access.hpp>
#endif

namespace godot {

class PPDPackNative : public Node {
    GDCLASS(PPDPackNative, Node);

protected:
    static void _bind_methods();

private:
    Ref<FileAccess> file;
    bool valid = true;
    std::vector<String> file_names;
    std::vector<uint32_t> file_sizes;
    std::vector<size_t> file_offsets;

public:
    PPDPackNative();
    ~PPDPackNative();

    bool load(const String &p_path);
    int get_file_index(const String &p_file_name);
    PackedByteArray get_file_data(int p_index);
    PackedStringArray get_files() const;
};

} // namespace godot

#endif // HB_PPD_PACK_HPP
