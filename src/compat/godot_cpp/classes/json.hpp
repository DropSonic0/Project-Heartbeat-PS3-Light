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
        while (pos < s.length() && isspace(s[pos])) pos++;
        if (pos >= s.length()) return Variant();

        if (s[pos] == '{') {
            Dictionary d;
            pos++;
            while (pos < s.length()) {
                while (pos < s.length() && isspace(s[pos])) pos++;
                if (s[pos] == '}') { pos++; break; }
                if (s[pos] == ',') { pos++; continue; }
                
                // Key
                if (s[pos] != '\"') return Variant();
                pos++;
                size_t start = pos;
                while (pos < s.length() && s[pos] != '\"') pos++;
                String key = s.substr(start, pos - start).c_str();
                pos++;
                
                while (pos < s.length() && (isspace(s[pos]) || s[pos] == ':')) pos++;
                
                d[key] = _parse_value(s, pos);
            }
            return d;
        } else if (s[pos] == '[') {
            Array a;
            pos++;
            while (pos < s.length()) {
                while (pos < s.length() && isspace(s[pos])) pos++;
                if (s[pos] == ']') { pos++; break; }
                if (s[pos] == ',') { pos++; continue; }
                a.append(_parse_value(s, pos));
            }
            return a;
        } else if (s[pos] == '\"') {
            pos++;
            size_t start = pos;
            while (pos < s.length() && s[pos] != '\"') pos++;
            String val = s.substr(start, pos - start).c_str();
            pos++;
            return val;
        } else {
            // Number or Bool
            size_t start = pos;
            while (pos < s.length() && !isspace(s[pos]) && s[pos] != ',' && s[pos] != '}' && s[pos] != ']') pos++;
            std::string val = s.substr(start, pos - start);
            if (val == "true") return true;
            if (val == "false") return false;
            if (val.find('.') != std::string::npos) return std::atof(val.c_str());
            return (long long)std::atoll(val.c_str());
        }
    }

public:
    int parse(const String& p_json_string) {
        size_t pos = 0;
        data = _parse_value(p_json_string, pos);
        return 0;
    }
    Variant get_data() const { return data; }
};

}

#endif
