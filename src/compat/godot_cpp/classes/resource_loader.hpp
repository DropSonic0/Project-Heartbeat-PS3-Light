#ifndef COMPAT_RESOURCE_LOADER_HPP
#define COMPAT_RESOURCE_LOADER_HPP

#include "object.hpp"
#include "resource.hpp"
#include "image.hpp"
#include "texture2d.hpp"
#include "font_variation.hpp"
#include "project_settings.hpp"
#include "file_access.hpp"
#include "../variant/utility_functions.hpp"

namespace godot {

class ResourceLoader : public Object {
public:
    static ResourceLoader* get_singleton() {
        static ResourceLoader* singleton = new ResourceLoader();
        return singleton;
    }

    Ref<Resource> load(const String& p_path, const String& p_type_hint = "") {
        UtilityFunctions::print("ResourceLoader: Attempting to load: " + p_path);

        // Prioritize raw physical assets in USRDIR if they exist
        String physical_path = p_path;
        if (p_path.begins_with("res://")) {
            physical_path = p_path.replace("res://", "/dev_hdd0/game/PROJECTHB/USRDIR/");
        }
        if (FileAccess::file_exists(physical_path)) {
            UtilityFunctions::print("ResourceLoader: Using prioritized physical resource: " + physical_path);
            return _load_internal(physical_path, p_type_hint);
        }
        
        String path = p_path;
        if (!FileAccess::file_exists(path)) {
            // Try fallback between .svg and .png for icons
            if (path.ends_with(".svg")) {
                path = path.replace(".svg", ".png");
            } else if (path.ends_with(".png")) {
                path = path.replace(".png", ".svg");
            }
            
            // Check if there is an .import file instead
            if (!FileAccess::file_exists(path)) {
                if (FileAccess::file_exists(p_path + ".import")) {
                    path = p_path + ".import";
                }
            }
        }

        if (FileAccess::file_exists(path)) {
            UtilityFunctions::print("ResourceLoader: Found resource at: " + path);
            
            if (path.ends_with(".import")) {
                UtilityFunctions::print("ResourceLoader: Parsing import file: " + path);
                Ref<FileAccess> f = FileAccess::open(path, FileAccess::READ);
                if (f.is_valid()) {
                    String content = f->get_as_text();
                    size_t path_pos = content.find("path=\"res://");
                    if (path_pos != std::string::npos) {
                        size_t start = path_pos + 6; // skip path="
                        size_t end = content.find("\"", start);
                        if (end != std::string::npos) {
                            String redirected_path = content.substr(start, end - start);
                            UtilityFunctions::print("ResourceLoader: Redirecting to: " + redirected_path);
                            return load(redirected_path, p_type_hint);
                        }
                    }
                }
            }

            String path_lower = path.to_lower();
            if (path_lower.ends_with(".png") || path_lower.ends_with(".svg") || path_lower.ends_with(".webp") || path_lower.ends_with(".ctex")) {
                Ref<FileAccess> f = FileAccess::open(path, FileAccess::READ);
                if (f.is_valid()) {
                    PackedByteArray buffer = f->get_buffer((size_t)f->get_length());
                    Ref<Image> img = Image::load_from_buffer(buffer);
                    if (img.is_valid()) {
                        return (Ref<Resource>)img;
                    }
                }

                // Fallback: Return an Image object which the driver can upload to VRAM
                Ref<Image> img = Image::create(64, 64, false, 0);
                PackedByteArray data;
                data.resize(64 * 64 * 4);
                for (int i=0; i<data.size(); i+=4) {
                    data[i] = 255;   // R
                    data[i+1] = 0;   // G
                    data[i+2] = 255; // B
                    data[i+3] = 255; // A
                }
                img->set_data(data);
                return (Ref<Resource>)img;
            }
            
            if (path_lower.ends_with(".ttf") || path_lower.ends_with(".otf") || path_lower.ends_with(".fontdata")) {
                Ref<FileAccess> f = FileAccess::open(path, FileAccess::READ);
                if (f.is_valid()) {
                    PackedByteArray buffer = f->get_buffer((size_t)f->get_length());
                    Ref<FontVariation> font;
                    font.instantiate();
                    font->set_data(buffer);
                    return (Ref<Resource>)font;
                }
                Ref<FontVariation> font;
                font.instantiate();
                return (Ref<Resource>)font;
            }

            Ref<Resource> res;
            res.instantiate();
            return res;
        }

        UtilityFunctions::print("ResourceLoader: Resource not found: " + p_path);
        return Ref<Resource>(NULL);
    }

private:
    Ref<Resource> _load_internal(const String& path, const String& p_type_hint) {
        String path_lower = path.to_lower();
        if (path_lower.ends_with(".png") || path_lower.ends_with(".svg") || path_lower.ends_with(".webp") || path_lower.ends_with(".ctex")) {
            Ref<FileAccess> f = FileAccess::open(path, FileAccess::READ);
            if (f.is_valid()) {
                PackedByteArray buffer = f->get_buffer((size_t)f->get_length());
                Ref<Image> img = Image::load_from_buffer(buffer);
                if (img.is_valid()) {
                    return (Ref<Resource>)img;
                }
            }
        }
        
        if (path_lower.ends_with(".ttf") || path_lower.ends_with(".otf") || path_lower.ends_with(".fontdata")) {
            Ref<FileAccess> f = FileAccess::open(path, FileAccess::READ);
            if (f.is_valid()) {
                PackedByteArray buffer = f->get_buffer((size_t)f->get_length());
                Ref<FontVariation> font;
                font.instantiate();
                font->set_data(buffer);
                return (Ref<Resource>)font;
            }
        }

        Ref<Resource> res;
        res.instantiate();
        return res;
    }
};

}

#endif
