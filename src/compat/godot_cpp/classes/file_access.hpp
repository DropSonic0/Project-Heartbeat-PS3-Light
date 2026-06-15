#ifndef COMPAT_FILE_ACCESS_HPP
#define COMPAT_FILE_ACCESS_HPP

#include "ref_counted.hpp"
#include "../variant/variant.hpp"
#include <fstream>
#include <sstream>

namespace godot {

class FileAccess : public RefCounted {
    GDCLASS(FileAccess, RefCounted);

    String path;
public:
    enum ModeFlags {
        READ = 1,
        WRITE = 2,
        READ_WRITE = 3,
        WRITE_READ = 7
    };

    static Ref<FileAccess> open(const String& p_path, ModeFlags p_mode) {
        FileAccess *fa = new FileAccess();
        fa->path = p_path;
        // Check if file exists/can be opened for reading
        if (p_mode == READ) {
            std::ifstream f(p_path.c_str());
            if (!f.good()) {
                delete fa;
                return Ref<FileAccess>(NULL);
            }
        }
        return Ref<FileAccess>(fa);
    }

    static bool file_exists(const String& p_path) {
        std::ifstream f(p_path.c_str());
        return f.good();
    }

    String get_as_text() const {
        std::ifstream f(path.c_str());
        if (!f.is_open()) return "";
        std::stringstream ss;
        ss << f.rdbuf();
        return ss.str().c_str();
    }
};

}

#endif
