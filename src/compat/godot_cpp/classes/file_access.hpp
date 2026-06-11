#ifndef COMPAT_FILE_ACCESS_HPP
#define COMPAT_FILE_ACCESS_HPP

#include "ref_counted.hpp"
#include "../variant/variant.hpp"

namespace godot {

class FileAccess : public RefCounted {
    GDCLASS(FileAccess, RefCounted);

public:
    enum ModeFlags {
        READ = 1,
        WRITE = 2,
        READ_WRITE = 3,
        WRITE_READ = 7
    };

    static Ref<FileAccess> open(const String& p_path, ModeFlags p_mode) {
        return Ref<FileAccess>(new FileAccess());
    }

    String get_as_text() const { return ""; }
};

}

#endif
