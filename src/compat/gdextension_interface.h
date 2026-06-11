#ifndef GDEXTENSION_INTERFACE_H
#define GDEXTENSION_INTERFACE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t GDExtensionBool;
typedef void *GDExtensionClassLibraryPtr;
typedef void *GDExtensionTypePtr;
typedef void *GDExtensionPtr;

typedef void* (*GDExtensionInterfaceGetProcAddress)(const char *p_get_proc_address);

typedef enum {
    GDEXTENSION_INITIALIZATION_CORE,
    GDEXTENSION_INITIALIZATION_SERVERS,
    GDEXTENSION_INITIALIZATION_SCENE,
    GDEXTENSION_INITIALIZATION_EDITOR,
    GDEXTENSION_INITIALIZATION_MAX,
} GDExtensionInitializationLevel;

typedef struct {
    GDExtensionInitializationLevel minimum_initialization_level;
    void *userdata;
    void (*initialize)(void *userdata, GDExtensionInitializationLevel p_level);
    void (*deinitialize)(void *userdata, GDExtensionInitializationLevel p_level);
} GDExtensionInitialization;

#ifndef GDE_EXPORT
#if defined(_WIN32) || defined(__PPU__) || defined(__SNC__)
#define GDE_EXPORT
#else
#define GDE_EXPORT __attribute__((visibility("default")))
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
