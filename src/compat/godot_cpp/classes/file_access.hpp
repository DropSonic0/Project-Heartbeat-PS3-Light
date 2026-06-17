#ifndef COMPAT_FILE_ACCESS_HPP
#define COMPAT_FILE_ACCESS_HPP

#include "ref_counted.hpp"
#include "../variant/variant.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include "project_settings.hpp"

namespace godot {

class FileAccess : public RefCounted {
    GDCLASS(FileAccess, RefCounted);

    PCKFileEntry pck_entry;
    String pck_container_path;
    bool is_pck_file = false;
    uint64_t pck_pos = 0;

public:
    enum ModeFlags {
        READ = 1,
        WRITE = 2,
        READ_WRITE = 3,
        WRITE_READ = 7
    };

private:
    String path;
    mutable std::ifstream f_in;
    mutable std::ofstream f_out;
    ModeFlags mode;

public:
    FileAccess() : mode(READ) {}

    static Ref<FileAccess> open(const String& p_path, ModeFlags p_mode) {
        FileAccess *fa = new FileAccess();
        fa->path = p_path;
        fa->mode = p_mode;
        if (p_mode == READ) {
            String physical_path = p_path;
            if (p_path.begins_with("res://")) {
                String pck_path;
                PCKFileEntry entry = ProjectSettings::get_singleton()->find_file_in_packs(p_path, pck_path);
                if (entry.size > 0) {
                    fa->is_pck_file = true;
                    fa->pck_entry = entry;
                    fa->pck_container_path = pck_path;
                    fa->f_in.open(pck_path.c_str(), std::ios::binary);
                    fa->f_in.seekg(entry.offset);
                    if (fa->f_in.is_open()) return Ref<FileAccess>(fa);
                }
                physical_path = p_path.replace("res://", "/dev_hdd0/game/PROJECTHB/USRDIR/");
            }

            fa->f_in.open(physical_path.c_str(), std::ios::binary);
            if (!fa->f_in.is_open()) {
                delete fa;
                return Ref<FileAccess>(NULL);
            }
        } else if (p_mode == WRITE) {
            fa->f_out.open(p_path.c_str(), std::ios::binary);
            if (!fa->f_out.is_open()) {
                delete fa;
                return Ref<FileAccess>(NULL);
            }
        }
        return Ref<FileAccess>(fa);
    }

    static bool file_exists(const String& p_path) {
        String physical_path = p_path;
        if (p_path.begins_with("res://")) {
            String pck_path;
            if (ProjectSettings::get_singleton()->find_file_in_packs(p_path, pck_path).size > 0) return true;
            physical_path = p_path.replace("res://", "/dev_hdd0/game/PROJECTHB/USRDIR/");
        }
        std::ifstream f(physical_path.c_str());
        return f.good();
    }

    String get_as_text() const {
        if (is_pck_file) {
            f_in.clear();
            uint64_t current_pos = f_in.tellg();
            f_in.seekg((std::streampos)pck_entry.offset);
            if (!f_in.good()) {
                UtilityFunctions::print("FileAccess: ERROR: seekg failed for " + path + " at " + String::num_int64(pck_entry.offset));
            }
            std::vector<char> buffer(pck_entry.size);
            f_in.read(&buffer[0], (std::streamsize)pck_entry.size);
            if ((uint64_t)f_in.gcount() != pck_entry.size) {
                 UtilityFunctions::print("FileAccess: ERROR: read failed for " + path + ". Expected " + String::num_int64(pck_entry.size) + " got " + String::num_int64((uint64_t)f_in.gcount()));
            }
            
            if (path.find("song.json") != std::string::npos) {
                UtilityFunctions::print("FileAccess: Read " + String::num_int64(pck_entry.size) + " bytes from PCK at " + String::num_int64(pck_entry.offset) + " for " + path);
                if (pck_entry.size > 0) {
                    std::string hex_dump = "";
                    for (int i=0; i<std::min((int)pck_entry.size, 64); i++) {
                        char b[4];
                        sprintf(b, "%02X ", (unsigned char)buffer[i]);
                        hex_dump += b;
                    }
                    UtilityFunctions::print("FileAccess: Hex (first 64): " + String(hex_dump.c_str()));
                }
            }

            f_in.clear();
            f_in.seekg(current_pos);
            return String(std::string(&buffer[0], pck_entry.size));
        }
        if (!f_in.is_open()) {
            std::ifstream f(path.c_str());
            if (!f.is_open()) return "";
            std::stringstream ss;
            ss << f.rdbuf();
            return ss.str().c_str();
        }
        size_t pos = f_in.tellg();
        f_in.seekg(0, std::ios::beg);
        std::stringstream ss;
        ss << f_in.rdbuf();
        f_in.clear();
        f_in.seekg(pos);
        return ss.str().c_str();
    }

    uint8_t get_8() const {
        uint8_t v = 0;
        f_in.read((char*)&v, (std::streamsize)1);
        return v;
    }

    uint16_t get_16() const {
        uint8_t b[2];
        f_in.read((char*)b, 2);
        return (uint16_t)b[0] << 8 | (uint16_t)b[1];
    }

    uint32_t get_32() const {
        uint8_t b[4];
        f_in.read((char*)b, 4);
        return (uint32_t)b[0] << 24 | (uint32_t)b[1] << 16 | (uint32_t)b[2] << 8 | (uint32_t)b[3];
    }

    uint32_t get_32_le() const {
        uint8_t b[4];
        f_in.read((char*)b, 4);
        return (uint32_t)b[0] | ((uint32_t)b[1] << 8) | ((uint32_t)b[2] << 16) | ((uint32_t)b[3] << 24);
    }

    uint64_t get_64() const {
        uint8_t b[8];
        f_in.read((char*)b, 8);
        return ((uint64_t)b[0] << 56) | ((uint64_t)b[1] << 48) | ((uint64_t)b[2] << 40) | ((uint64_t)b[3] << 32) |
               ((uint64_t)b[4] << 24) | ((uint64_t)b[5] << 16) | ((uint64_t)b[6] << 8) | ((uint64_t)b[7]);
    }

    uint64_t get_64_le() const {
        uint8_t b[8];
        f_in.read((char*)b, 8);
        return (uint64_t)b[0] | 
               ((uint64_t)b[1] << 8) | 
               ((uint64_t)b[2] << 16) | 
               ((uint64_t)b[3] << 24) |
               ((uint64_t)b[4] << 32) | 
               ((uint64_t)b[5] << 40) | 
               ((uint64_t)b[6] << 48) | 
               ((uint64_t)b[7] << 56);
    }

    float get_float() const {
        uint32_t v = get_32();
        return *((float*)&v);
    }

    String get_line() const {
        std::string line;
        std::getline(f_in, line);
        if (!line.empty() && line[line.length() - 1] == '\r') {
            line.resize(line.length() - 1);
        }
        return line.c_str();
    }

    PackedByteArray get_buffer(size_t p_len) const {
        PackedByteArray res;
        if (is_pck_file) {
            uint64_t current_pos = (uint64_t)f_in.tellg();
            if (current_pos < pck_entry.offset) {
                p_len = 0;
            } else {
                uint64_t current_relative = current_pos - pck_entry.offset;
                if (current_relative >= pck_entry.size) {
                    p_len = 0;
                } else if (current_relative + (uint64_t)p_len > pck_entry.size) {
                    p_len = (size_t)(pck_entry.size - current_relative);
                }
            }
        }
        res.resize(p_len);
        if (p_len > 0) {
            f_in.read((char*)&res[0], (std::streamsize)p_len);
        }
        return res;
    }

    size_t get_position() const {
        if (mode == READ) {
            if (is_pck_file) return (size_t)((uint64_t)f_in.tellg() - pck_entry.offset);
            return (size_t)f_in.tellg();
        }
        return (size_t)f_out.tellp();
    }

    void seek(size_t p_pos) {
        if (mode == READ) {
            f_in.clear();
            if (is_pck_file) f_in.seekg((std::streampos)(pck_entry.offset + p_pos));
            else f_in.seekg((std::streampos)p_pos);
        } else {
            f_out.seekp((std::streampos)p_pos);
        }
    }

    bool eof_reached() const {
        if (is_pck_file) return (uint64_t)f_in.tellg() >= pck_entry.offset + pck_entry.size;
        return f_in.eof() || (f_in.peek() == EOF);
    }

    void store_8(uint8_t p_dest) {
        f_out.write((char*)&p_dest, 1);
    }

    void store_32(uint32_t p_dest) {
#ifdef __PPU__
        f_out.write((char*)&p_dest, 4);
#else
        uint32_t v = ((p_dest & 0x000000FF) << 24) | 
                     ((p_dest & 0x0000FF00) << 8)  | 
                     ((p_dest & 0x00FF0000) >> 8)  | 
                     ((p_dest & 0xFF000000) >> 24);
        f_out.write((char*)&v, 4);
#endif
    }

    void store_buffer(const PackedByteArray& p_buffer) {
        if (p_buffer.size() > 0) {
            f_out.write((char*)&p_buffer[0], p_buffer.size());
        }
    }

    void store_string(const String& p_string) {
        f_out.write(p_string.c_str(), p_string.size());
    }
};

}

#endif
