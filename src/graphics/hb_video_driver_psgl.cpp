#include "hb_video_driver_psgl.hpp"
#include "compat/godot_cpp/classes/font_variation.hpp"
#include <godot_cpp/variant/utility_functions.hpp>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>

#ifdef __PPU__
#include <cell/cell_fs.h>
#include <sysutil/sysutil_sysparam.h>
#include <sysutil/sysutil_common.h>
#include <sys/sys_time.h>
#include <sys/process.h>
#endif

namespace godot {

#ifdef __PPU__
static PSGLcontext* _psgl_context = nullptr;
static PSGLdevice* _psgl_device = nullptr;
static GLuint _gl_width = 0;
static GLuint _gl_height = 0;
static bool _should_exit = false;
static CellPadData _pad_data;
static bool _pad_initialized = false;
static std::map<uintptr_t, GLuint> _texture_cache;

// 8x8 debug font
static const unsigned char _debug_font[96][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // space
    {0x18,0x3C,0x3C,0x18,0x18,0x00,0x18,0x00}, // !
    {0x6C,0x6C,0x6C,0x00,0x00,0x00,0x00,0x00}, // "
    {0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00}, // #
    {0x18,0x3E,0x60,0x3C,0x06,0x7C,0x18,0x00}, // $
    {0x00,0xC6,0xCC,0x18,0x30,0x66,0xC6,0x00}, // %
    {0x38,0x6C,0x38,0x76,0xDC,0xCC,0x76,0x00}, // &
    {0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00}, // '
    {0x18,0x30,0x60,0x60,0x60,0x30,0x18,0x00}, // (
    {0x60,0x30,0x18,0x18,0x18,0x30,0x60,0x00}, // )
    {0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00}, // *
    {0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00}, // +
    {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30}, // ,
    {0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00}, // -
    {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00}, // .
    {0x02,0x06,0x0C,0x18,0x30,0x60,0xC0,0x00}, // /
    {0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00}, // 0
    {0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00}, // 1
    {0x3C,0x66,0x06,0x0C,0x18,0x30,0x7E,0x00}, // 2
    {0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00}, // 3
    {0x0C,0x1C,0x3C,0x6C,0xFE,0x0C,0x0C,0x00}, // 4
    {0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00}, // 5
    {0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00}, // 6
    {0x7E,0x06,0x0C,0x18,0x30,0x30,0x30,0x00}, // 7
    {0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00}, // 8
    {0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00}, // 9
    {0x00,0x18,0x18,0x00,0x18,0x18,0x00,0x00}, // :
    {0x00,0x18,0x18,0x00,0x18,0x18,0x30,0x00}, // ;
    {0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x00}, // <
    {0x00,0x00,0x7E,0x00,0x7E,0x00,0x00,0x00}, // =
    {0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x00}, // >
    {0x3C,0x66,0x06,0x0C,0x18,0x00,0x18,0x00}, // ?
    {0x3C,0x66,0x06,0x5E,0x66,0x66,0x3C,0x00}, // @
    {0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00}, // A
    {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00}, // B
    {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00}, // C
    {0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00}, // D
    {0x7E,0x60,0x60,0x78,0x60,0x60,0x7E,0x00}, // E
    {0x7E,0x60,0x60,0x78,0x60,0x60,0x60,0x00}, // F
    {0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00}, // G
    {0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00}, // H
    {0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00}, // I
    {0x1E,0x0C,0x0C,0x0C,0x0C,0xCC,0x78,0x00}, // J
    {0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00}, // K
    {0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00}, // L
    {0xC6,0xEE,0xFE,0xFE,0xD6,0xC6,0xC6,0x00}, // M
    {0xC6,0xE6,0xF6,0xDE,0xCE,0xC6,0xC6,0x00}, // N
    {0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, // O
    {0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00}, // P
    {0x3C,0x66,0x66,0x66,0x66,0x6C,0x36,0x00}, // Q
    {0x7C,0x66,0x66,0x7C,0x6C,0x66,0x66,0x00}, // R
    {0x3C,0x66,0x30,0x18,0x0C,0x66,0x3C,0x00}, // S
    {0x7E,0x5A,0x18,0x18,0x18,0x18,0x18,0x00}, // T
    {0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, // U
    {0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00}, // V
    {0xC6,0xC6,0xD6,0xFE,0xFE,0xEE,0xC6,0x00}, // W
    {0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00}, // X
    {0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00}, // Y
    {0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00}, // Z
    {0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00}, // [
    {0xC0,0x60,0x30,0x18,0x0C,0x06,0x03,0x00}, // \ .
    {0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00}, // ]
    {0x18,0x3C,0x66,0x00,0x00,0x00,0x00,0x00}, // ^
    {0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00}, // _
    {0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00}, // `
    {0x00,0x00,0x3C,0x06,0x3E,0x66,0x3E,0x00}, // a
    {0x60,0x60,0x7C,0x66,0x66,0x66,0x7C,0x00}, // b
    {0x00,0x00,0x3C,0x60,0x60,0x66,0x3C,0x00}, // c
    {0x06,0x06,0x3E,0x66,0x66,0x66,0x3E,0x00}, // d
    {0x00,0x00,0x3C,0x66,0x7E,0x60,0x3C,0x00}, // e
    {0x1C,0x30,0x7C,0x30,0x30,0x30,0x30,0x00}, // f
    {0x00,0x00,0x3E,0x66,0x66,0x3E,0x06,0x3C}, // g
    {0x60,0x60,0x7C,0x66,0x66,0x66,0x66,0x00}, // h
    {0x18,0x00,0x38,0x18,0x18,0x18,0x3C,0x00}, // i
    {0x06,0x00,0x0E,0x06,0x06,0x66,0x3C,0x00}, // j
    {0x60,0x60,0x66,0x6C,0x78,0x6C,0x66,0x00}, // k
    {0x38,0x18,0x18,0x18,0x18,0x18,0x3C,0x00}, // l
    {0x00,0x00,0xAC,0xD6,0xD6,0xD6,0xD6,0x00}, // m
    {0x00,0x00,0x7C,0x66,0x66,0x66,0x66,0x00}, // n
    {0x00,0x00,0x3C,0x66,0x66,0x66,0x3C,0x00}, // o
    {0x00,0x00,0x7C,0x66,0x66,0x7C,0x60,0x60}, // p
    {0x00,0x00,0x3E,0x66,0x66,0x3E,0x06,0x06}, // q
    {0x00,0x00,0x7C,0x66,0x60,0x60,0x60,0x00}, // r
    {0x00,0x00,0x3E,0x60,0x3C,0x06,0x7C,0x00}, // s
    {0x30,0x30,0x7C,0x30,0x30,0x36,0x1C,0x00}, // t
    {0x00,0x00,0x66,0x66,0x66,0x66,0x3E,0x00}, // u
    {0x00,0x00,0x66,0x66,0x66,0x3C,0x18,0x00}, // v
    {0x00,0x00,0xD6,0xD6,0xD6,0xFE,0x6C,0x00}, // w
    {0x00,0x00,0x66,0x3C,0x18,0x3C,0x66,0x00}, // x
    {0x00,0x00,0x66,0x66,0x66,0x3E,0x06,0x3C}, // y
    {0x00,0x00,0x7E,0x0C,0x18,0x30,0x7E,0x00}, // z
    {0x0C,0x18,0x18,0x70,0x18,0x18,0x0C,0x00}, // {
    {0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00}, // |
    {0x30,0x18,0x18,0x0E,0x18,0x18,0x30,0x00}, // }
    {0x00,0x00,0x00,0x32,0x4C,0x00,0x00,0x00}, // ~
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}  // del
};

static void _sysutil_callback(uint64_t status, uint64_t param, void* userdata) {
    (void)param;
    (void)userdata;
    if (status == CELL_SYSUTIL_REQUEST_EXITGAME) {
        _should_exit = true;
    }
}
#endif

bool HBVideoDriverPSGL::initialize() {
#ifdef __PPU__
    if (_psgl_device) {
        return true;
    }
    UtilityFunctions::print("Initializing PSGL...");

    PSGLinitOptions options;
    options.enable = PSGL_INIT_MAX_SPUS | PSGL_INIT_INITIALIZE_SPUS | PSGL_INIT_HOST_MEMORY_SIZE;
    options.maxSPUs = 1;
    options.initializeSPUs = false;
    options.persistentMemorySize = 0;
    options.transientMemorySize = 0;
    options.errorConsole = 0;
    options.fifoSize = 0;
    options.hostMemorySize = 8 * 1024 * 1024;

    psglInit(&options);

    PSGLdeviceParameters params;
    params.enable = PSGL_DEVICE_PARAMETERS_COLOR_FORMAT | 
                    PSGL_DEVICE_PARAMETERS_DEPTH_FORMAT | 
                    PSGL_DEVICE_PARAMETERS_MULTISAMPLING_MODE | 
                    PSGL_DEVICE_PARAMETERS_BUFFERING_MODE | 
                    PSGL_DEVICE_PARAMETERS_RESC_ADJUST_ASPECT_RATIO;
    params.bufferingMode = PSGL_BUFFERING_MODE_TRIPLE;
    params.colorFormat = GL_ARGB_SCE;
    params.depthFormat = GL_NONE;
    params.multisamplingMode = GL_MULTISAMPLING_NONE_SCE;

    // Default to Auto Resolution if possible
    _psgl_device = psglCreateDeviceExtended(&params);
    if (!_psgl_device) {
        UtilityFunctions::print("Failed to create PSGL device.");
        return false;
    }

    psglGetDeviceDimensions(_psgl_device, &_gl_width, &_gl_height);
    
    _psgl_context = psglCreateContext();
    if (!_psgl_context) {
        UtilityFunctions::print("Failed to create PSGL context.");
        return false;
    }

    psglMakeCurrent(_psgl_context, _psgl_device);
    psglResetCurrentContext();

    glViewport(0, 0, _gl_width, _gl_height);
    glScissor(0, 0, _gl_width, _gl_height);

    cellSysutilRegisterCallback(0, _sysutil_callback, nullptr);

    if (cellPadInit(1) == CELL_OK) {
        _pad_initialized = true;
    }
    
    char buf[128];
    sprintf(buf, "PSGL Initialized successfully. Resolution: %dx%d", _gl_width, _gl_height);
    UtilityFunctions::print(buf);
    
    return true;
#else
    return true;
#endif
}

void HBVideoDriverPSGL::terminate() {
#ifdef __PPU__
    clear_texture_cache();
    if (_pad_initialized) {
        cellPadEnd();
        _pad_initialized = false;
    }
    if (_psgl_context) {
        psglDestroyContext(_psgl_context);
        _psgl_context = nullptr;
    }
    if (_psgl_device) {
        psglDestroyDevice(_psgl_device);
        _psgl_device = nullptr;
    }
    psglExit();
#endif
}

void HBVideoDriverPSGL::swap_buffers() {
#ifdef __PPU__
    if (!_psgl_device) return;
    psglSwap();
#endif
}

void HBVideoDriverPSGL::clear_buffer() {
#ifdef __PPU__
    if (!_psgl_device) return;
    glClearColor(0.129f, 0.071f, 0.259f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
}

void HBVideoDriverPSGL::update_system_callbacks() {
#ifdef __PPU__
    cellSysutilCheckCallback();
    if (_pad_initialized) {
        CellPadInfo2 pad_info;
        if (cellPadGetInfo2(&pad_info) == CELL_OK) {
            if (pad_info.port_status[0] & CELL_PAD_STATUS_CONNECTED) {
                cellPadGetData(0, &_pad_data);
            }
        }
    }
#endif
}

bool HBVideoDriverPSGL::should_exit() {
#ifdef __PPU__
    return _should_exit;
#else
    return false;
#endif
}

Vector2 HBVideoDriverPSGL::get_window_size() {
#ifdef __PPU__
    return Vector2(_gl_width, _gl_height);
#else
    return Vector2(1920, 1080);
#endif
}

void HBVideoDriverPSGL::draw_rect(const Rect2& p_rect, const Color& p_color) {
    draw_parallelogram(p_rect, 0.0f, p_color);
}

void HBVideoDriverPSGL::draw_parallelogram(const Rect2& p_rect, float p_slant, const Color& p_color) {
#ifdef __PPU__
    if (!_psgl_device) return;
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrthof(0, _gl_width, _gl_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float x1 = p_rect.position.x;
    float y1 = p_rect.position.y;
    float x2 = p_rect.position.x + p_rect.size.x;
    float y2 = p_rect.position.y + p_rect.size.y;

    GLfloat vertices[] = {
        x1 + p_slant, y1,
        x2 + p_slant, y1,
        x1, y2,
        x2, y2
    };

    glColor4f(p_color.r, p_color.g, p_color.b, p_color.a);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
#endif
}

static void _draw_text_internal(const String& p_text, const Vector2& p_pos, const Color& p_color, float p_scale) {
#ifdef __PPU__
    glColor4f(p_color.r, p_color.g, p_color.b, p_color.a);
    float cur_x = p_pos.x;
    float char_w = 8.0f * p_scale;

    std::vector<GLfloat> vertices;
    vertices.reserve(p_text.size() * 8 * 8 * 12); // Worst case: all pixels set, 2 triangles (6 verts) * 2 coords per char

    for (size_t i = 0; i < p_text.size(); i++) {
        char c = p_text[i];
        if (c < 32 || c > 127) continue;
        int idx = c - 32;

        for (int y = 0; y < 8; y++) {
            unsigned char row = _debug_font[idx][y];
            for (int x = 0; x < 8; x++) {
                if (row & (1 << (7 - x))) {
                    float px = cur_x + x * p_scale;
                    float py = p_pos.y + y * p_scale;
                    
                    // Triangle Strip for each pixel: 4 vertices
                    // We'll use independent triangles for simplicity in batching: 6 vertices per pixel
                    float x1 = px;
                    float y1 = py;
                    float x2 = px + p_scale;
                    float y2 = py + p_scale;

                    vertices.push_back(x1); vertices.push_back(y1);
                    vertices.push_back(x2); vertices.push_back(y1);
                    vertices.push_back(x1); vertices.push_back(y2);

                    vertices.push_back(x2); vertices.push_back(y1);
                    vertices.push_back(x2); vertices.push_back(y2);
                    vertices.push_back(x1); vertices.push_back(y2);
                }
            }
        }
        cur_x += char_w + 1.0f * p_scale;
    }

    if (!vertices.empty()) {
        glVertexPointer(2, GL_FLOAT, 0, &vertices[0]);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);
    }
#endif
}

void HBVideoDriverPSGL::draw_text(const String& p_text, const Vector2& p_pos, const Color& p_color, float p_scale, bool p_shadow) {
#ifdef __PPU__
    if (!_psgl_device) return;
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrthof(0, _gl_width, _gl_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnableClientState(GL_VERTEX_ARRAY);

    if (p_shadow) {
        _draw_text_internal(p_text, p_pos + Vector2(2 * p_scale, 2 * p_scale), Color(0, 0, 0, p_color.a * 0.5f), p_scale);
    }
    _draw_text_internal(p_text, p_pos, p_color, p_scale);

    glDisableClientState(GL_VERTEX_ARRAY);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
#endif
}

void HBVideoDriverPSGL::draw_text_with_font(const Ref<FontVariation>& p_font, const String& p_text, const Vector2& p_pos, int p_size, const Color& p_color, bool p_shadow, bool p_center) {
    bool rendered = false;
    if (p_font.is_valid()) {
        Ref<Image> img = p_font->render_text(p_text, p_size);
        if (img.is_valid()) {
            Vector2 draw_pos = p_pos;
            if (p_center) {
                draw_pos.x -= img->get_width() / 2.0f;
                draw_pos.y -= img->get_height() / 2.0f;
            }

            if (p_shadow) {
                draw_texture(img, Rect2(draw_pos + Vector2(2, 2), Vector2(img->get_width(), img->get_height())), Color(0, 0, 0, p_color.a * 0.5f));
            }

            draw_texture(img, Rect2(draw_pos, Vector2(img->get_width(), img->get_height())), p_color);
            rendered = true;
        }
    }

    if (!rendered) {
        // Fallback to debug font if rendering failed
        Vector2 draw_pos = p_pos;
        if (p_center) {
            draw_pos.x -= (p_text.length() * p_size) / 2.0f;
            draw_pos.y -= p_size / 2.0f;
        }
        draw_text(p_text, draw_pos, p_color, (float)p_size / 8.0f, p_shadow);
    }
}

void HBVideoDriverPSGL::draw_texture(const Ref<Image>& p_image, const Rect2& p_rect, const Color& p_modulate) {
#ifdef __PPU__
    if (!_psgl_device || p_image.is_null()) return;

    GLuint tex_id = 0;
    uintptr_t img_ptr = (uintptr_t)p_image.ptr();
    if (_texture_cache.count(img_ptr)) {
        tex_id = _texture_cache[img_ptr];
    } else {
        glGenTextures(1, &tex_id);
        glBindTexture(GL_TEXTURE_2D, tex_id);
        PackedByteArray data = p_image->get_data();
        const void* data_ptr = data.empty() ? nullptr : &data[0];
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_image->get_width(), p_image->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data_ptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        _texture_cache[img_ptr] = tex_id;
    }

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrthof(0, _gl_width, _gl_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float x1 = p_rect.position.x;
    float y1 = p_rect.position.y;
    float x2 = p_rect.position.x + p_rect.size.x;
    float y2 = p_rect.position.y + p_rect.size.y;

    GLfloat vertices[] = {
        x1, y1,
        x2, y1,
        x1, y2,
        x2, y2
    };

    GLfloat tex_coords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };

    glColor4f(p_modulate.r, p_modulate.g, p_modulate.b, p_modulate.a);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
#endif
}

void HBVideoDriverPSGL::clear_texture_cache() {
#ifdef __PPU__
    for (std::map<uintptr_t, GLuint>::iterator it = _texture_cache.begin(); it != _texture_cache.end(); ++it) {
        glDeleteTextures(1, &it->second);
    }
    _texture_cache.clear();
#endif
}

bool HBVideoDriverPSGL::is_button_pressed(int p_button) {
#ifdef __PPU__
    if (!_pad_initialized || _pad_data.len <= CELL_PAD_BTN_OFFSET_DIGITAL2) return false;
    // The PS3 PAD report stores digital buttons in two bytes.
    // Digital 1: Select (0x01), L3 (0x02), R3 (0x04), Start (0x08), Up (0x10), Right (0x20), Down (0x40), Left (0x80)
    // Digital 2: L2 (0x01), R2 (0x02), L1 (0x04), R1 (0x08), Triangle (0x10), Circle (0x20), Cross (0x40), Square (0x80)
    //
    // In the CellPadData structure, these are separate uint16_t elements at offsets DIGITAL1 (2) and DIGITAL2 (3).
    // The CELL_PAD_CTRL_* constants (like CELL_PAD_CTRL_CROSS = 0x4000) are defined assuming
    // a 16-bit word where Digital 2 is the high byte and Digital 1 is the low byte.
    uint16_t d1 = _pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & 0xFF;
    uint16_t d2 = _pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & 0xFF;
    uint16_t buttons = (d2 << 8) | d1;
    return (buttons & p_button) != 0;
#else
    return false;
#endif
}

} // namespace godot
