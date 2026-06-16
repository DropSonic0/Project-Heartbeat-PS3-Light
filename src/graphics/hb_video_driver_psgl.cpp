#include "hb_video_driver_psgl.hpp"
#include <godot_cpp/variant/utility_functions.hpp>
#include <stdio.h>
#include <string.h>

#ifdef __PPU__
#include <cell/cell_fs.h>
#include <sysutil/sysutil_sysparam.h>
#include <sys/sys_time.h>
#include <sys/process.h>
#endif

using namespace godot;

#ifdef __PPU__
static PSGLcontext* _psgl_context = nullptr;
static PSGLdevice* _psgl_device = nullptr;
static GLuint _gl_width = 0;
static GLuint _gl_height = 0;
#endif

bool HBVideoDriverPSGL::initialize() {
#ifdef __PPU__
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
    psglSwap();
#endif
}

void HBVideoDriverPSGL::clear_buffer() {
#ifdef __PPU__
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
}
