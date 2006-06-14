/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2006 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

#ifndef _SDL_sysvideo_h
#define _SDL_sysvideo_h

#include "SDL_mouse.h"
#define SDL_PROTOTYPES_ONLY
#include "SDL_syswm.h"
#undef SDL_PROTOTYPES_ONLY

/* This file prototypes the video driver implementation.
   This is designed to be easily converted to C++ in the future.
 */

#if SDL_VIDEO_OPENGL
#include "SDL_opengl.h"
#endif /* SDL_VIDEO_OPENGL */

/* The SDL video driver */

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Texture SDL_Texture;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_RenderDriver SDL_RenderDriver;
typedef struct SDL_VideoDisplay SDL_VideoDisplay;
typedef struct SDL_VideoDevice SDL_VideoDevice;

/* Define the SDL texture structure */
struct SDL_Texture
{
    Uint32 id;

    Uint32 format;      /**< The pixel format of the texture */
    int access;         /**< SDL_TextureAccess */
    int w;              /**< The width of the texture */
    int h;              /**< The height of the texture */

    SDL_Renderer *renderer;

    void *driverdata;   /**< Driver specific texture representation */

    SDL_Texture *next;
};

/* Define the SDL renderer structure */
struct SDL_Renderer
{
    int (*CreateTexture) (SDL_Renderer * renderer, SDL_Texture * texture);
    int (*QueryTexturePixels) (SDL_Renderer * renderer, SDL_Texture * texture,
                               void **pixels, int *pitch);
    int (*SetTexturePalette) (SDL_Renderer * renderer, SDL_Texture * texture,
                              SDL_Color * colors, int firstcolor,
                              int ncolors);
    int (*UpdateTexture) (SDL_Renderer * renderer, SDL_Texture * texture,
                          SDL_Rect * rect, const void *pixels, int pitch);
    int (*LockTexture) (SDL_Renderer * renderer, SDL_Texture * texture,
                        SDL_Rect * rect, int markDirty, void **pixels,
                        int *pitch);
    void (*UnlockTexture) (SDL_Renderer * renderer, SDL_Texture * texture);
    void (*DirtyTexture) (SDL_Renderer * renderer, SDL_Texture * texture,
                          int numrects, SDL_Rect * rects);
    void (*SelectRenderTexture) (SDL_Renderer * renderer,
                                 SDL_Texture * texture);
    void (*RenderFill) (SDL_Renderer * renderer, SDL_Rect * rect,
                        Uint32 color);
    int (*RenderCopy) (SDL_Renderer * renderer, SDL_Texture * texture,
                       SDL_Rect * srcrect, SDL_Rect * dstrect, int blendMode,
                       int scaleMode);
    int (*RenderReadPixels) (SDL_Renderer * renderer, SDL_Rect * rect,
                             void *pixels, int pitch);
    int (*RenderWritePixels) (SDL_Renderer * renderer, SDL_Rect * rect,
                              const void *pixels, int pitch);
    void (*RenderPresent) (SDL_Renderer * renderer);
    void (*DestroyTexture) (SDL_Renderer * renderer, SDL_Texture * texture);

    void (*DestroyRenderer) (SDL_Renderer * renderer);

    /* The current renderer info */
    SDL_RendererInfo info;

    /* The window associated with the renderer */
    SDL_Window *window;

    void *driverdata;
};

/* Define the SDL render driver structure */
struct SDL_RenderDriver
{
    SDL_Renderer *(*CreateRenderer) (SDL_Window * window, Uint32 flags);

    /* Info about the renderer capabilities */
    SDL_RendererInfo info;
};

/* Define the SDL window structure, corresponding to toplevel windows */
struct SDL_Window
{
    Uint32 id;

    char *title;
    int x, y;
    int w, h;
    Uint32 flags;

    Uint16 *gamma;

    SDL_VideoDisplay *display;
    SDL_Renderer *renderer;

    void *userdata;
    void *driverdata;
};

/* Define the SDL display structure
   This corresponds to physical monitors attached to the system.
 */
struct SDL_VideoDisplay
{
    int num_display_modes;
    SDL_DisplayMode *display_modes;
    SDL_DisplayMode desktop_mode;
    SDL_DisplayMode current_mode;

    int num_render_drivers;
    SDL_RenderDriver *render_drivers;

    int num_windows;
    SDL_Window *windows;

    SDL_Renderer *current_renderer;

    /* The hash list of textures */
    SDL_Texture *textures[64];

    void *driverdata;
};

/* Define the SDL video driver structure */
#define _THIS	SDL_VideoDevice *_this

struct SDL_VideoDevice
{
    /* * * */
    /* The name of this video driver */
    const char *name;

    /* * * */
    /* Initialization/Query functions */

    /* Initialize the native video subsystem, filling in the list
       of displays for this driver, returning 0 or -1 if there's an error.
     */
    int (*VideoInit) (_THIS);

    /* * * */
    /* Display functions
     */
    /* Setting the display mode is independent of creating windows,
     * so when the display mode is changed, all existing windows
     * should have their data updated accordingly, including the
     * display surfaces associated with them.
     */
    int (*SetDisplayMode) (_THIS, const SDL_DisplayMode * mode);

    /* Sets the color entries { firstcolor .. (firstcolor+ncolors-1) }
       of the physical palette to those in 'colors'.  The return value
       is 0 if all entries could be set properly or -1 otherwise.
     */
    int (*SetDisplayColors) (_THIS, int firstcolor, int ncolors,
                             SDL_Color * colors);

    /* * * */
    /* Window functions
     */
    int (*CreateWindow) (_THIS, SDL_Window * window);
    int (*CreateWindowFrom) (_THIS, SDL_Window * window, void *data);
    void (*SetWindowTitle) (_THIS, SDL_Window * window);
    void (*SetWindowPosition) (_THIS, SDL_Window * window);
    void (*SetWindowSize) (_THIS, SDL_Window * window);
    void (*ShowWindow) (_THIS, SDL_Window * window);
    void (*HideWindow) (_THIS, SDL_Window * window);
    void (*RaiseWindow) (_THIS, SDL_Window * window);
    void (*MaximizeWindow) (_THIS, SDL_Window * window);
    void (*MinimizeWindow) (_THIS, SDL_Window * window);
    void (*RestoreWindow) (_THIS, SDL_Window * window);
    void (*SetWindowGrab) (_THIS, SDL_Window * window);
    void (*DestroyWindow) (_THIS, SDL_Window * window);

    /* Get some platform dependent window information */
      SDL_bool(*GetWindowWMInfo) (_THIS, SDL_Window * window,
                                  SDL_SysWMinfo * info);

    /* Reverse the effects VideoInit() -- called if VideoInit() fails
       or if the application is shutting down the video subsystem.
     */
    void (*VideoQuit) (_THIS);

    /* * * */
    /* Gamma support */

    /* Set the gamma correction directly (emulated with gamma ramps) */
    int (*SetGamma) (_THIS, float red, float green, float blue);

    /* Get the gamma correction directly (emulated with gamma ramps) */
    int (*GetGamma) (_THIS, float *red, float *green, float *blue);

    /* Set the gamma ramp */
    int (*SetGammaRamp) (_THIS, Uint16 * ramp);

    /* Get the gamma ramp */
    int (*GetGammaRamp) (_THIS, Uint16 * ramp);

    /* * * */
    /* OpenGL support */

    /* Sets the dll to use for OpenGL and loads it */
    int (*GL_LoadLibrary) (_THIS, const char *path);

    /* Retrieves the address of a function in the gl library */
    void *(*GL_GetProcAddress) (_THIS, const char *proc);

    /* Get attribute information from the windowing system. */
    int (*GL_GetAttribute) (_THIS, SDL_GLattr attrib, int *value);

    /* Make the context associated with this driver current */
    int (*GL_MakeCurrent) (_THIS);

    /* Swap the current buffers in double buffer mode. */
    void (*GL_SwapBuffers) (_THIS);

    /* OpenGL functions for glSDL */
#if SDL_VIDEO_OPENGL
#if !defined(__WIN32__)
#define WINAPI
#endif
#define SDL_PROC(ret,func,params) ret (WINAPI *func) params;
#include "SDL_glfuncs.h"
#undef SDL_PROC

    /* Texture id */
    GLuint texture;

    int is_32bit;
#endif

    /* Determine whether the mouse should be in relative mode or not.
       This function is called when the input grab state or cursor
       visibility state changes.
       If the cursor is not visible, and the input is grabbed, the
       driver can place the mouse in relative mode, which may result
       in higher accuracy sampling of the pointer motion.
     */
    void (*CheckMouseMode) (_THIS);

    /* * * */
    /* Event manager functions */

    /* Handle any queued OS events */
    void (*PumpEvents) (_THIS);

    /* * * */
    /* Data common to all drivers */
    int num_displays;
    SDL_VideoDisplay *displays;
    int current_display;
    Uint32 next_object_id;

    /* Driver information flags */

    /* * * */
    /* Data used by the GL drivers */
    struct
    {
        int red_size;
        int green_size;
        int blue_size;
        int alpha_size;
        int depth_size;
        int buffer_size;
        int stencil_size;
        int double_buffer;
        int accum_red_size;
        int accum_green_size;
        int accum_blue_size;
        int accum_alpha_size;
        int stereo;
        int multisamplebuffers;
        int multisamplesamples;
        int accelerated;
        int swap_control;
        int driver_loaded;
        char driver_path[256];
        void *dll_handle;
    } gl_config;

    /* * * */
    /* Data private to this driver */
    struct SDL_PrivateVideoData *hidden;
    struct SDL_PrivateGLData *gl_data;

    /* * * */
    /* The function used to dispose of this structure */
    void (*free) (_THIS);
};
#undef _THIS

typedef struct VideoBootStrap
{
    const char *name;
    const char *desc;
    int (*available) (void);
    SDL_VideoDevice *(*create) (int devindex);
} VideoBootStrap;

#if SDL_VIDEO_DRIVER_QUARTZ
extern VideoBootStrap QZ_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_X11
extern VideoBootStrap X11_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_DGA
extern VideoBootStrap DGA_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_NANOX
extern VideoBootStrap NX_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_IPOD
extern VideoBootStrap iPod_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_QTOPIA
extern VideoBootStrap Qtopia_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_WSCONS
extern VideoBootStrap WSCONS_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_FBCON
extern VideoBootStrap FBCON_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_DIRECTFB
extern VideoBootStrap DirectFB_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_PS2GS
extern VideoBootStrap PS2GS_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_GGI
extern VideoBootStrap GGI_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_VGL
extern VideoBootStrap VGL_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_SVGALIB
extern VideoBootStrap SVGALIB_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_GAPI
extern VideoBootStrap GAPI_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_WINDIB
extern VideoBootStrap WINDIB_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_DDRAW
extern VideoBootStrap DIRECTX_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_BWINDOW
extern VideoBootStrap BWINDOW_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_TOOLBOX
extern VideoBootStrap TOOLBOX_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_DRAWSPROCKET
extern VideoBootStrap DSp_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_CYBERGRAPHICS
extern VideoBootStrap CGX_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_PHOTON
extern VideoBootStrap ph_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_EPOC
extern VideoBootStrap EPOC_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_XBIOS
extern VideoBootStrap XBIOS_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_GEM
extern VideoBootStrap GEM_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_PICOGUI
extern VideoBootStrap PG_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_DC
extern VideoBootStrap DC_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_RISCOS
extern VideoBootStrap RISCOS_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_OS2FS
extern VideoBootStrap OS2FSLib_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_AALIB
extern VideoBootStrap AALIB_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_DUMMY
extern VideoBootStrap DUMMY_bootstrap;
#endif
#if SDL_VIDEO_DRIVER_GLSDL
extern VideoBootStrap glSDL_bootstrap;
#endif

#define SDL_CurrentDisplay	(_this->displays[_this->current_display])
#define SDL_CurrentWindow	(SDL_CurrentDisplay.windows[0])

extern SDL_VideoDevice *SDL_GetVideoDevice();
extern int SDL_AddBasicVideoDisplay(const SDL_DisplayMode * desktop_mode);
extern int SDL_AddVideoDisplay(const SDL_VideoDisplay * display);
extern void SDL_AddDisplayMode(int displayIndex,
                               const SDL_DisplayMode * mode);
extern void SDL_AddRenderDriver(int displayIndex,
                                const SDL_RenderDriver * driver);

#endif /* _SDL_sysvideo_h */

/* vi: set ts=4 sw=4 expandtab: */
