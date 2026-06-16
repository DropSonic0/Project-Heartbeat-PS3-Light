#include "hb_video_driver_psgl.hpp"
#include <godot_cpp/variant/utility_functions.hpp>
#include <stdio.h>
#include <string.h>

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
    glClearColor(0.2f, 0.2f, 0.4f, 1.0f); // Dark blueish clear color to show it's alive
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

void HBVideoDriverPSGL::draw_rect(const Rect2& p_rect, const Color& p_color) {
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
        x1, y1,
        x2, y1,
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

bool HBVideoDriverPSGL::is_button_pressed(int p_button) {
#ifdef __PPU__
    if (!_pad_initialized) return false;
    return (_pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & p_button) != 0 || 
           (_pad_data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & p_button) != 0;
#else
    return false;
#endif
}

} // namespace godot
