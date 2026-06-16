#ifndef COMPAT_DIR_ACCESS_HPP
#define COMPAT_DIR_ACCESS_HPP

#include "ref_counted.hpp"
#include "../variant/variant.hpp"
#include "project_settings.hpp"
#include <vector>
#include <string>

#ifdef __PPU__
#include <cell/cell_fs.h>
#else
// For development on PC if needed, but primarily for PS3
#include <dirent.h>
#endif

namespace godot {

class DirAccess : public RefCounted {
    GDCLASS(DirAccess, RefCounted);

    String path;
public:
    static Ref<DirAccess> open(const String& p_path) {
        Ref<DirAccess> da;
        da.instantiate();
        da->path = p_path;
        return da;
    }

    PackedStringArray get_directories() {
        PackedStringArray res;
        if (path.begins_with("res://")) {
            res = ProjectSettings::get_singleton()->get_directories_in_packs(path);
            return res; // Don't attempt physical scan for res://
        }
#ifdef __PPU__
        int fd;
        if (cellFsOpendir(path.c_str(), &fd) == 0) {
            CellFsDirent entry;
            uint64_t read;
            while (cellFsReaddir(fd, &entry, &read) == 0 && read > 0) {
                String name = entry.d_name;
                if (name == "." || name == "..") continue;
                
                String full_path = path;
                if (!full_path.ends_with("/")) full_path += "/";
                full_path += name;
                
                CellFsStat stat;
                if (cellFsStat(full_path.c_str(), &stat) == 0) {
                    if (stat.st_mode & CELL_FS_S_IFDIR) {
                        res.append(name);
                    }
                }
            }
            cellFsClosedir(fd);
        }
#endif
        return res;
    }

    PackedStringArray get_files() {
        PackedStringArray res;
        if (path.begins_with("res://")) {
            res = ProjectSettings::get_singleton()->get_files_in_packs(path);
            return res; // Don't attempt physical scan for res://
        }
#ifdef __PPU__
        int fd;
        if (cellFsOpendir(path.c_str(), &fd) == 0) {
            CellFsDirent entry;
            uint64_t read;
            while (cellFsReaddir(fd, &entry, &read) == 0 && read > 0) {
                String name = entry.d_name;
                
                String full_path = path;
                if (!full_path.ends_with("/")) full_path += "/";
                full_path += name;
                
                CellFsStat stat;
                if (cellFsStat(full_path.c_str(), &stat) == 0) {
                    if (stat.st_mode & CELL_FS_S_IFREG) {
                        res.append(name);
                    }
                }
            }
            cellFsClosedir(fd);
        }
#endif
        return res;
    }

    bool dir_exists(const String& p_dir) {
        if (p_dir.begins_with("res://")) {
            // Simplified: if it has files in packs, it exists
            return !ProjectSettings::get_singleton()->get_files_in_packs(p_dir).empty() || 
                   !ProjectSettings::get_singleton()->get_directories_in_packs(p_dir).empty();
        }
#ifdef __PPU__
        CellFsStat stat;
        if (cellFsStat(p_dir.c_str(), &stat) == CELL_FS_SUCCEEDED) {
            return (stat.st_mode & CELL_FS_S_IFDIR);
        }
#endif
        return false;
    }

    bool file_exists(const String& p_file) {
        if (p_file.begins_with("res://")) {
            String pck_path;
            if (ProjectSettings::get_singleton()->find_file_in_packs(p_file, pck_path).size > 0) return true;
        }
#ifdef __PPU__
        CellFsStat stat;
        if (cellFsStat(p_file.c_str(), &stat) == CELL_FS_SUCCEEDED) {
            return (stat.st_mode & CELL_FS_S_IFREG);
        }
#endif
        return false;
    }
};

}

#endif
