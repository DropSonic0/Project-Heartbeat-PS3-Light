#ifndef COMPAT_FILE_ACCESS_HPP
#define COMPAT_FILE_ACCESS_HPP

#include "ref_counted.hpp"
#include "../variant/variant.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <stdint.h>

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
            fa->f_in.open(p_path.c_str(), std::ios::binary);
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
        std::ifstream f(p_path.c_str());
        return f.good();
    }

    String get_as_text() const {
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
        f_in.read((char*)&v, 1);
        return v;
    }

    uint16_t get_16() const {
        uint16_t v = 0;
        f_in.read((char*)&v, 2);
#ifdef __PPU__
        // PS3 is Big Endian, and MIDI/PPD data is Big Endian
        return v;
#else
        // x86 is Little Endian, swap to get Big Endian value from file
        return ((v & 0xFF) << 8) | ((v & 0xFF00) >> 8);
#endif
    }

    uint32_t get_32() const {
        uint32_t v = 0;
        f_in.read((char*)&v, 4);
#ifdef __PPU__
        return v;
#else
        return ((v & 0x000000FF) << 24) | 
               ((v & 0x0000FF00) << 8)  | 
               ((v & 0x00FF0000) >> 8)  | 
               ((v & 0xFF000000) >> 24);
#endif
    }

    uint64_t get_64() const {
        uint64_t v = 0;
        f_in.read((char*)&v, 8);
#ifdef __PPU__
        return v;
#else
        return ((v & 0x00000000000000FFULL) << 56) |
               ((v & 0x000000000000FF00ULL) << 40) |
               ((v & 0x0000000000FF0000ULL) << 24) |
               ((v & 0x00000000FF000000ULL) << 8)  |
               ((v & 0x000000FF00000000ULL) >> 8)  |
               ((v & 0x0000FF0000000000ULL) >> 24) |
               ((v & 0x00FF000000000000ULL) >> 40) |
               ((v & 0xFF00000000000000ULL) >> 56);
#endif
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
        res.resize(p_len);
        if (p_len > 0) {
            f_in.read((char*)&res[0], p_len);
        }
        return res;
    }

    size_t get_position() const {
        if (mode == READ) return (size_t)f_in.tellg();
        return (size_t)f_out.tellp();
    }

    void seek(size_t p_pos) {
        if (mode == READ) {
            f_in.clear();
            f_in.seekg(p_pos);
        } else {
            f_out.seekp(p_pos);
        }
    }

    bool eof_reached() const {
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
