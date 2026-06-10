#include "hb_folder.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBFolderNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("has_subfolder", "name"), &HBFolderNative::has_subfolder);
    ClassDB::bind_method(D_METHOD("get_subfolder", "name"), &HBFolderNative::get_subfolder);
    ClassDB::bind_method(D_METHOD("set_folder_name", "name"), &HBFolderNative::set_folder_name);
    ClassDB::bind_method(D_METHOD("get_folder_name"), &HBFolderNative::get_folder_name);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "folder_name"), "set_folder_name", "get_folder_name");
}

HBFolderNative::HBFolderNative() {
    serializable_fields.append("songs");
    serializable_fields.append("subfolders");
    serializable_fields.append("sort_mode");
    serializable_fields.append("folder_name");
}

HBFolderNative::~HBFolderNative() {}

bool HBFolderNative::has_subfolder(const String &p_name) const {
    for (int i = 0; i < subfolders.size(); i++) {
        Ref<HBFolderNative> folder = subfolders[i];
        if (folder.is_valid() && folder->get_folder_name() == p_name) {
            return true;
        }
    }
    return false;
}

Ref<HBFolderNative> HBFolderNative::get_subfolder(const String &p_name) const {
    for (int i = 0; i < subfolders.size(); i++) {
        Ref<HBFolderNative> folder = subfolders[i];
        if (folder.is_valid() && folder->get_folder_name() == p_name) {
            return folder;
        }
    }
    return nullptr;
}
