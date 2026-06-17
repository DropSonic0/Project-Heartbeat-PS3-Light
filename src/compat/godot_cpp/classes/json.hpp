#ifndef COMPAT_JSON_HPP
#define COMPAT_JSON_HPP

#include "ref_counted.hpp"
#include "../variant/variant.hpp"
#include <string>
#include <vector>
#include <ctype.h>
#include <stdlib.h>

namespace godot {

class JSON : public RefCounted {
    GDCLASS(JSON, RefCounted);

    Variant data;

    // Very basic JSON parser for song metadata
    Variant _parse_value(const std::string& s, size_t& pos) {
        while (pos < s.length() && (isspace((unsigned char)s[pos]) || s[pos] == 0)) pos++;
        if (pos >= s.length()) return Variant();

        // UtilityFunctions::print("JSON: Parsing at pos " + String::num(pos) + " char: " + String(std::string(1, s[pos]).c_str()));

        if (s[pos] == '{') {
            Dictionary d;
            pos++;
            while (pos < s.length()) {
                while (pos < s.length() && (isspace((unsigned char)s[pos]) || s[pos] == 0)) pos++;
                if (s[pos] == '}') { pos++; break; }
                if (s[pos] == ',') { pos++; continue; }
                
                // Key
                if (s[pos] != '\"') return Variant();
                pos++;
                size_t start = pos;
                while (pos < s.length() && s[pos] != '\"') pos++;
                String key = s.substr(start, pos - start).c_str();
                pos++;
                
                while (pos < s.length() && (isspace((unsigned char)s[pos]) || s[pos] == ':' || s[pos] == 0)) pos++;
                
                d[key] = _parse_value(s, pos);
            }
            return d;
        } else if (s[pos] == '[') {
            Array a;
            pos++;
            while (pos < s.length()) {
                while (pos < s.length() && (isspace((unsigned char)s[pos]) || s[pos] == 0)) pos++;
                if (s[pos] == ']') { pos++; break; }
                if (s[pos] == ',') { pos++; continue; }
                a.append(_parse_value(s, pos));
            }
            return a;
        } else if (s[pos] == '\"') {
            pos++;
            std::string val = "";
            while (pos < s.length() && s[pos] != '\"') {
                if (s[pos] == '\\' && pos + 1 < s.length()) {
                    pos++;
                    if (s[pos] == 'n') val += '\n';
                    else if (s[pos] == 'r') val += '\r';
                    else if (s[pos] == 't') val += '\t';
                    else if (s[pos] == '\"') val += '\"';
                    else if (s[pos] == '\\') val += '\\';
                    else val += s[pos];
                } else {
                    val += s[pos];
                }
                pos++;
            }
            if (pos < s.length()) pos++; // Skip closing quote
            return String(val.c_str());
        } else {
            // Number or Bool or Null
            size_t start = pos;
            while (pos < s.length() && !isspace((unsigned char)s[pos]) && s[pos] != ',' && s[pos] != '}' && s[pos] != ']' && s[pos] != 0) pos++;
            std::string val = s.substr(start, pos - start);
            if (val == "true") return true;
            if (val == "false") return false;
            if (val == "null") return Variant();
            if (val.find('.') != std::string::npos || val.find('e') != std::string::npos || val.find('E') != std::string::npos) return std::atof(val.c_str());
            return (long long)std::atoll(val.c_str());
        }
    }

public:
    int parse(const String& p_json_string) {
        size_t pos = 0;
        // Skip UTF-8 BOM if present
        if (p_json_string.length() >= 3 && 
            (unsigned char)p_json_string[0] == 0xEF && 
            (unsigned char)p_json_string[1] == 0xBB && 
            (unsigned char)p_json_string[2] == 0xBF) {
            pos = 3;
        }
        data = _parse_value(p_json_string, pos);
        
        while (pos < p_json_string.length() && (isspace((unsigned char)p_json_string[pos]) || p_json_string[pos] == 0)) pos++;
        if (pos < p_json_string.length()) {
            // UtilityFunctions::print("JSON: Warning: trailing data at pos " + String::num(pos));
        }
        
        if (data.get_type() == Variant::NIL && p_json_string.length() > 0) {
             UtilityFunctions::print("JSON: Failed to parse anything from string of length " + String::num(p_json_string.length()));
             return 1;
        }

        return 0;
    }
    Variant get_data() const { return data; }
};

}

#endif
