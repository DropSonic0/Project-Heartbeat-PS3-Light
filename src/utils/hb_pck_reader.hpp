#ifndef HB_PCK_READER_HPP
#define HB_PCK_READER_HPP

#include "compat/godot_cpp/variant/variant.hpp"
#include "compat/godot_cpp/classes/project_settings.hpp"
#include "compat/godot_cpp/classes/file_access.hpp"
#include <map>

namespace godot {

class PCKReader : public Object {
    String pck_path;
    std::map<String, PCKFileEntry> files;
    std::map<String, String> lowercase_files;

public:
    PCKReader();
    bool load_pck(const String& p_path);
    bool file_exists(const String& p_path) const;
    PCKFileEntry get_file_entry(const String& p_path) const;
    String get_pck_path() const { return pck_path; }
    void set_file_entry(const String& p_path, const PCKFileEntry& p_entry) { files[p_path] = p_entry; }

    std::vector<String> get_all_files() const {
        std::vector<String> res;
        for (std::map<String, PCKFileEntry>::const_iterator it = files.begin(); it != files.end(); ++it) {
            res.push_back(it->first);
        }
        return res;
    }
};

}

#endif
