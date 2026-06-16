#ifndef HB_FOLDER_HPP
#define HB_FOLDER_HPP

#include "hb_serializable.hpp"

namespace godot {

class HBFolderNative : public HBSerializableNative {
    GDCLASS(HBFolderNative, HBSerializableNative);

protected:
    static void _bind_methods();

private:
    Array songs;
    Array subfolders;
    String sort_mode = "title";
    String folder_name;

public:
    HBFolderNative();
    ~HBFolderNative();

    bool has_subfolder(const String &p_name) const;
    Ref<HBFolderNative> get_subfolder(const String &p_name) const;

    void set_folder_name(const String &p_name) { folder_name = p_name; }
    String get_folder_name() const { return folder_name; }

    virtual String get_serialized_type() const GD_OVERRIDE { return "Folder"; }
};

} // namespace godot

#endif // HB_FOLDER_HPP
