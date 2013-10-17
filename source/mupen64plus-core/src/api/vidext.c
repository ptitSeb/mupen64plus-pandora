/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus-core - api/vidext.c                                       *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2009 Richard Goedeken                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       * 
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
                       
/* This file contains the Core video extension functions which will be exported
 * outside of the core library.
 */

#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#define M64P_CORE_PROTOTYPES 1
#include "m64p_types.h"
#include "m64p_vidext.h"
#include "vidext.h"
#include "callbacks.h"
#include "../osd/osd.h"

#if SDL_VERSION_ATLEAST(2,0,0)
#include "vidext_sdl2_compat.h"
#endif

#ifdef PANDORA
#define USE_EGL_SDL 1
#ifdef USE_EGL_SDL
#include "eglport.h"
#else
#include <EGL/egl.h>
#include <SDL_syswm.h>
SDL_SysWMinfo sysWmInfo;
static EGLSurface eglSurface = NULL;
static EGLContext eglContext = NULL;
static EGLConfig  eglConfig = NULL;
static EGLDisplay eglDisplay = NULL;
#endif
static int useFB = 0;
#endif

/* local variables */
static m64p_video_extension_functions l_ExternalVideoFuncTable = {10, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static int l_VideoExtensionActive = 0;
static int l_VideoOutputActive = 0;
static int l_Fullscreen = 0;
static SDL_Surface *l_pScreen = NULL;

/* global function for use by frontend.c */
m64p_error OverrideVideoFunctions(m64p_video_extension_functions *VideoFunctionStruct)
{
    /* check input data */
    if (VideoFunctionStruct == NULL)
        return M64ERR_INPUT_ASSERT;
    if (VideoFunctionStruct->Functions < 11)
        return M64ERR_INPUT_INVALID;

    /* disable video extension if any of the function pointers are NULL */
    if (VideoFunctionStruct->VidExtFuncInit == NULL ||
        VideoFunctionStruct->VidExtFuncQuit == NULL ||
        VideoFunctionStruct->VidExtFuncListModes == NULL ||
        VideoFunctionStruct->VidExtFuncSetMode == NULL ||
        VideoFunctionStruct->VidExtFuncGLGetProc == NULL ||
        VideoFunctionStruct->VidExtFuncGLSetAttr == NULL ||
        VideoFunctionStruct->VidExtFuncGLGetAttr == NULL ||
        VideoFunctionStruct->VidExtFuncGLSwapBuf == NULL ||
        VideoFunctionStruct->VidExtFuncSetCaption == NULL ||
        VideoFunctionStruct->VidExtFuncToggleFS == NULL ||
        VideoFunctionStruct->VidExtFuncResizeWindow == NULL)
    {
        l_ExternalVideoFuncTable.Functions = 11;
        memset(&l_ExternalVideoFuncTable.VidExtFuncInit, 0, 11 * sizeof(void *));
        l_VideoExtensionActive = 0;
        return M64ERR_SUCCESS;
    }

    /* otherwise copy in the override function pointers */
    memcpy(&l_ExternalVideoFuncTable, VideoFunctionStruct, sizeof(m64p_video_extension_functions));
    l_VideoExtensionActive = 1;
    return M64ERR_SUCCESS;
}

int VidExt_InFullscreenMode(void)
{
    return l_Fullscreen;
}

int VidExt_VideoRunning(void)
{
    return l_VideoOutputActive;
}

/* video extension functions to be called by the video plugin */
EXPORT m64p_error CALL VidExt_Init(void)
{
    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
        return (*l_ExternalVideoFuncTable.VidExtFuncInit)();

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) == -1)
    {
        DebugMessage(M64MSG_ERROR, "SDL video subsystem init failed: %s", SDL_GetError());
        return M64ERR_SYSTEM_FAIL;
    }

    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL VidExt_Quit(void)
{
    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
    {
        m64p_error rval = (*l_ExternalVideoFuncTable.VidExtFuncQuit)();
        if (rval == M64ERR_SUCCESS)
        {
            l_VideoOutputActive = 0;
            StateChanged(M64CORE_VIDEO_MODE, M64VIDEO_NONE);
        }
        return rval;
    }

    if (!SDL_WasInit(SDL_INIT_VIDEO))
        return M64ERR_NOT_INIT;
	#ifdef PANDORA
	#ifdef USE_EGL_SDL
	EGL_Close();
	#else
	if (eglDisplay) {
		eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (eglContext)
			eglDestroyContext(eglDisplay, eglContext);
		if (eglSurface)
			eglDestroySurface(eglDisplay, eglSurface);
		eglTerminate(eglDisplay);
	}
	eglDisplay = NULL;
	eglContext = NULL;
	eglSurface = NULL;
	#endif
	#endif
    SDL_ShowCursor(SDL_ENABLE);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    l_pScreen = NULL;
    l_VideoOutputActive = 0;
    StateChanged(M64CORE_VIDEO_MODE, M64VIDEO_NONE);

    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL VidExt_ListFullscreenModes(m64p_2d_size *SizeArray, int *NumSizes)
{
    const SDL_VideoInfo *videoInfo;
    unsigned int videoFlags;
    SDL_Rect **modes;
    int i;

    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
        return (*l_ExternalVideoFuncTable.VidExtFuncListModes)(SizeArray, NumSizes);

    if (!SDL_WasInit(SDL_INIT_VIDEO))
        return M64ERR_NOT_INIT;

    /* get a list of SDL video modes */
	#ifdef PANDORA
    videoFlags = SDL_FULLSCREEN;
	#else
    videoFlags = SDL_OPENGL | SDL_FULLSCREEN;
	#endif

    if ((videoInfo = SDL_GetVideoInfo()) == NULL)
    {
        DebugMessage(M64MSG_ERROR, "SDL_GetVideoInfo query failed: %s", SDL_GetError());
        return M64ERR_SYSTEM_FAIL;
    }
	#ifndef PANDORA
    if(videoInfo->hw_available)
        videoFlags |= SDL_HWSURFACE;
    else
    #endif
        videoFlags |= SDL_SWSURFACE;

    modes = SDL_ListModes(NULL, videoFlags);

    if (modes == (SDL_Rect **) 0 || modes == (SDL_Rect **) -1)
    {
        DebugMessage(M64MSG_WARNING, "No fullscreen SDL video modes available");
        *NumSizes = 0;
        return M64ERR_SUCCESS;
    }

    i = 0;
    while (i < *NumSizes && modes[i] != NULL)
    {
        SizeArray[i].uiWidth  = modes[i]->w;
        SizeArray[i].uiHeight = modes[i]->h;
        i++;
    }

    *NumSizes = i;

    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL VidExt_SetVideoMode(int Width, int Height, int BitsPerPixel, m64p_video_mode ScreenMode, m64p_video_flags Flags)
{
    const SDL_VideoInfo *videoInfo;
    int videoFlags = 0;
printf("VidExt_SetVideoMode(%i, %i, %i, %s, %x)\n", Width, Height, BitsPerPixel, (ScreenMode==M64VIDEO_WINDOWED)?"Windowed":"Fullscreen", Flags);
    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
    {
        m64p_error rval = (*l_ExternalVideoFuncTable.VidExtFuncSetMode)(Width, Height, BitsPerPixel, ScreenMode, Flags);
        l_Fullscreen = (rval == M64ERR_SUCCESS && ScreenMode == M64VIDEO_FULLSCREEN);
        l_VideoOutputActive = (rval == M64ERR_SUCCESS);
        if (l_VideoOutputActive)
        {
            StateChanged(M64CORE_VIDEO_MODE, ScreenMode);
            StateChanged(M64CORE_VIDEO_SIZE, (Width << 16) | Height);
        }
        return rval;
    }

    if (!SDL_WasInit(SDL_INIT_VIDEO)) {
printf("SDL was not init, aborting\n");
        return M64ERR_NOT_INIT;
    }

    /* Get SDL video flags to use */
    if (ScreenMode == M64VIDEO_WINDOWED)
    {
		#ifdef PANDORA
		videoFlags = 0;
		useFB = 0;
		#else
        videoFlags = SDL_OPENGL;
		#endif
        if (Flags & M64VIDEOFLAG_SUPPORT_RESIZING)
            videoFlags |= SDL_RESIZABLE;
    }
    else if (ScreenMode == M64VIDEO_FULLSCREEN)
    {
		#ifdef PANDORA
        videoFlags = SDL_FULLSCREEN;
		#else
        videoFlags = SDL_OPENGL | SDL_FULLSCREEN;
		#endif
    }
    else
    {
        return M64ERR_INPUT_INVALID;
    }

    if ((videoInfo = SDL_GetVideoInfo()) == NULL)
    {
        DebugMessage(M64MSG_ERROR, "SDL_GetVideoInfo query failed: %s", SDL_GetError());
        return M64ERR_SYSTEM_FAIL;
    }
	#ifndef PANDORA
    if (videoInfo->hw_available)
        videoFlags |= SDL_HWSURFACE;
    else
    #endif
        videoFlags |= SDL_SWSURFACE;

    /* set the mode */
    if (BitsPerPixel > 0)
        DebugMessage(M64MSG_INFO, "Setting %i-bit video mode: %ix%i", BitsPerPixel, Width, Height);
    else
        DebugMessage(M64MSG_INFO, "Setting video mode: %ix%i", Width, Height);

    l_pScreen = SDL_SetVideoMode(Width, Height, BitsPerPixel, videoFlags);
    if (l_pScreen == NULL)
    {
        DebugMessage(M64MSG_ERROR, "SDL_SetVideoMode failed: %s", SDL_GetError());
        return M64ERR_SYSTEM_FAIL;
    }
	#ifdef PANDORA
	// Setup EGL Context...
//	if ((Width==800) && (Height==480))	// 800x480 => FB and no X11
		useFB = 1;		
//	Width = 800;
//	Height = 480;
	#ifdef  USE_EGL_SDL
	EGL_Open(Width, Height);
	#else
    EGLint maj, min;
    SDL_VERSION(&sysWmInfo.version);
    SDL_GetWMInfo(&sysWmInfo);
    eglDisplay = eglGetDisplay( (useFB)? EGL_DEFAULT_DISPLAY:(EGLNativeDisplayType)sysWmInfo.info.x11.display );
	if (!eglInitialize(eglDisplay, &maj, &min)) {
		printf("eglinfo: eglInitialize failed\n");
	}
	printf("EGL v%i.%i initialized%s\n", maj, min, (useFB)?" using FB":"");
	EGLint attribs[] =
	{
		EGL_RED_SIZE,        5,
		EGL_GREEN_SIZE,      6,
		EGL_BLUE_SIZE,       5,
		EGL_DEPTH_SIZE,      16,
		EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SAMPLE_BUFFERS,  0,
		EGL_SAMPLES,         0,
		EGL_NONE
	};
	EGLint ctx_attribs[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};


	EGLint num_configs;
	if (!eglChooseConfig(eglDisplay, attribs, &eglConfig, 1, &num_configs) || (num_configs < 1))
	{
		printf("Could not find config for %s (perhaps this API is unsupported?)\n", "GLES2");
	}

	EGLint vid;
	if (!eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &vid))
	{
		printf("Could not get native visual ID from chosen config\n");
	}
	eglBindAPI(EGL_OPENGL_ES_API);

	eglContext=eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, ctx_attribs);
	eglSurface=eglCreateWindowSurface(eglDisplay, eglConfig, (NativeWindowType)((useFB)?NULL:sysWmInfo.info.x11.window), NULL);

	if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext))
	{
		printf("eglMakeCurrent() failed\n");
	}
	#endif	//eglport
	#endif

    SDL_ShowCursor(SDL_DISABLE);

    l_Fullscreen = (ScreenMode == M64VIDEO_FULLSCREEN);
    l_VideoOutputActive = 1;
    StateChanged(M64CORE_VIDEO_MODE, ScreenMode);
    StateChanged(M64CORE_VIDEO_SIZE, (Width << 16) | Height);
    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL VidExt_ResizeWindow(int Width, int Height)
{
    const SDL_VideoInfo *videoInfo;
    int videoFlags = 0;

    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
    {
        m64p_error rval;
        // shut down the OSD
        osd_exit();
        // re-create the OGL context
        rval = (*l_ExternalVideoFuncTable.VidExtFuncResizeWindow)(Width, Height);
        if (rval == M64ERR_SUCCESS)
        {
            StateChanged(M64CORE_VIDEO_SIZE, (Width << 16) | Height);
            // re-create the On-Screen Display
            osd_init(Width, Height);
        }
        return rval;
    }

    if (!l_VideoOutputActive || !SDL_WasInit(SDL_INIT_VIDEO))
        return M64ERR_NOT_INIT;

    if (l_Fullscreen)
    {
        DebugMessage(M64MSG_ERROR, "VidExt_ResizeWindow() called in fullscreen mode.");
        return M64ERR_INVALID_STATE;
    }

    /* Get SDL video flags to use */
	#ifdef PANDORA
	if (useFB)
		return M64ERR_INVALID_STATE;
    videoFlags = SDL_RESIZABLE;
	#else
    videoFlags = SDL_OPENGL | SDL_RESIZABLE;
	#endif
    if ((videoInfo = SDL_GetVideoInfo()) == NULL)
    {
        DebugMessage(M64MSG_ERROR, "SDL_GetVideoInfo query failed: %s", SDL_GetError());
        return M64ERR_SYSTEM_FAIL;
    }
    #ifndef PANDORA
    if (videoInfo->hw_available)
        videoFlags |= SDL_HWSURFACE;
    else
    #endif
        videoFlags |= SDL_SWSURFACE;

    // destroy the On-Screen Display
    osd_exit();

    /* set the re-sizing the screen will create a new OpenGL context */
    l_pScreen = SDL_SetVideoMode(Width, Height, 0, videoFlags);
    if (l_pScreen == NULL)
    {
        DebugMessage(M64MSG_ERROR, "SDL_SetVideoMode failed: %s", SDL_GetError());
        return M64ERR_SYSTEM_FAIL;
    }

    StateChanged(M64CORE_VIDEO_SIZE, (Width << 16) | Height);
    // re-create the On-Screen Display
    osd_init(Width, Height);
    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL VidExt_SetCaption(const char *Title)
{
    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
        return (*l_ExternalVideoFuncTable.VidExtFuncSetCaption)(Title);

    if (!SDL_WasInit(SDL_INIT_VIDEO))
        return M64ERR_NOT_INIT;

    SDL_WM_SetCaption(Title, "M64+ Video");

    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL VidExt_ToggleFullScreen(void)
{
    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
    {
        m64p_error rval = (*l_ExternalVideoFuncTable.VidExtFuncToggleFS)();
        if (rval == M64ERR_SUCCESS)
        {
            l_Fullscreen = !l_Fullscreen;
            StateChanged(M64CORE_VIDEO_MODE, l_Fullscreen ? M64VIDEO_FULLSCREEN : M64VIDEO_WINDOWED);
        }
        return rval;
    }

    if (!SDL_WasInit(SDL_INIT_VIDEO))
        return M64ERR_NOT_INIT;

    /* TODO:
     * SDL_WM_ToggleFullScreen doesn't work under Windows and others
     * (see http://wiki.libsdl.org/moin.cgi/FAQWindows for explanation).
     * Instead, we should call SDL_SetVideoMode with the SDL_FULLSCREEN flag.
     * (see http://sdl.beuc.net/sdl.wiki/SDL_SetVideoMode), but on Windows
     * this resets the OpenGL context and video plugins don't support it yet.
     * Uncomment the next line to test it: */
    //return VidExt_SetVideoMode(l_pScreen->w, l_pScreen->h, l_pScreen->format->BitsPerPixel, l_Fullscreen ? M64VIDEO_WINDOWED : M64VIDEO_FULLSCREEN);
/*	#ifdef PANDORA
	if (useFB)	// no effect on FB
		return M64ERR_INVALID_STATE;
	#endif*/
    if (SDL_WM_ToggleFullScreen(l_pScreen) == 1)
    {
        l_Fullscreen = !l_Fullscreen;
        StateChanged(M64CORE_VIDEO_MODE, l_Fullscreen ? M64VIDEO_FULLSCREEN : M64VIDEO_WINDOWED);
        return M64ERR_SUCCESS;
    }

    return M64ERR_SYSTEM_FAIL;
}

EXPORT void * CALL VidExt_GL_GetProcAddress(const char* Proc)
{
    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
        return (*l_ExternalVideoFuncTable.VidExtFuncGLGetProc)(Proc);

    if (!SDL_WasInit(SDL_INIT_VIDEO))
        return NULL;

    return SDL_GL_GetProcAddress(Proc);
}

typedef struct {
    m64p_GLattr m64Attr;
    SDL_GLattr sdlAttr;
} GLAttrMapNode;

static const GLAttrMapNode GLAttrMap[] = {
        { M64P_GL_DOUBLEBUFFER, SDL_GL_DOUBLEBUFFER },
        { M64P_GL_BUFFER_SIZE,  SDL_GL_BUFFER_SIZE },
        { M64P_GL_DEPTH_SIZE,   SDL_GL_DEPTH_SIZE },
        { M64P_GL_RED_SIZE,     SDL_GL_RED_SIZE },
        { M64P_GL_GREEN_SIZE,   SDL_GL_GREEN_SIZE },
        { M64P_GL_BLUE_SIZE,    SDL_GL_BLUE_SIZE },
        { M64P_GL_ALPHA_SIZE,   SDL_GL_ALPHA_SIZE },
#if SDL_VERSION_ATLEAST(1,3,0)
        { M64P_GL_SWAP_CONTROL, SDL_RENDERER_PRESENTVSYNC },
#else
        { M64P_GL_SWAP_CONTROL, SDL_GL_SWAP_CONTROL },
#endif
        { M64P_GL_MULTISAMPLEBUFFERS, SDL_GL_MULTISAMPLEBUFFERS },
        { M64P_GL_MULTISAMPLESAMPLES, SDL_GL_MULTISAMPLESAMPLES }};
static const int mapSize = sizeof(GLAttrMap) / sizeof(GLAttrMapNode);

EXPORT m64p_error CALL VidExt_GL_SetAttribute(m64p_GLattr Attr, int Value)
{
    int i;

    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
        return (*l_ExternalVideoFuncTable.VidExtFuncGLSetAttr)(Attr, Value);

    if (!SDL_WasInit(SDL_INIT_VIDEO))
        return M64ERR_NOT_INIT;

	#ifdef PANDORA
	return M64ERR_SUCCESS;
	#else
    for (i = 0; i < mapSize; i++)
    {
        if (GLAttrMap[i].m64Attr == Attr)
        {
            if (SDL_GL_SetAttribute(GLAttrMap[i].sdlAttr, Value) != 0)
                return M64ERR_SYSTEM_FAIL;
            return M64ERR_SUCCESS;
        }
    }

    return M64ERR_INPUT_INVALID;
    #endif
}

EXPORT m64p_error CALL VidExt_GL_GetAttribute(m64p_GLattr Attr, int *pValue)
{
    int i;

    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
        return (*l_ExternalVideoFuncTable.VidExtFuncGLGetAttr)(Attr, pValue);

    if (!SDL_WasInit(SDL_INIT_VIDEO))
        return M64ERR_NOT_INIT;

	#ifdef PANDORA
	return M64ERR_SUCCESS;
	#else
    for (i = 0; i < mapSize; i++)
    {
        if (GLAttrMap[i].m64Attr == Attr)
        {
            int NewValue = 0;
            if (SDL_GL_GetAttribute(GLAttrMap[i].sdlAttr, &NewValue) != 0)
                return M64ERR_SYSTEM_FAIL;
            *pValue = NewValue;
            return M64ERR_SUCCESS;
        }
    }

    return M64ERR_INPUT_INVALID;
    #endif
}

EXPORT m64p_error CALL VidExt_GL_SwapBuffers(void)
{
    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
        return (*l_ExternalVideoFuncTable.VidExtFuncGLSwapBuf)();

    if (!SDL_WasInit(SDL_INIT_VIDEO))
        return M64ERR_NOT_INIT;
	#ifdef PANDORA
	#ifdef USE_EGL_SDL
	EGL_SwapBuffers();
	#else
	eglSwapBuffers( eglDisplay, eglSurface );
	#endif
	#else
    SDL_GL_SwapBuffers();
	#endif
    return M64ERR_SUCCESS;
}


