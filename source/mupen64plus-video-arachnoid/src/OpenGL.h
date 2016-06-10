/******************************************************************************
 * Arachnoid Graphics Plugin for Mupen64Plus
 * http://bitbucket.org/wahrhaft/mupen64plus-video-arachnoid/
 *
 * Copyright (C) 2009 Jon Ring
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/
#ifndef OPENGL_H
#define OPENGL_H

#ifndef WIN32
    #define GL_GLEXT_PROTOTYPES
#endif
#if defined(__MACOSX__)
#include <OpenGL/gl.h>
#elif defined(__MACOS__)
#include <gl.h>
#else
#ifdef HAVE_GLES
#include <GLES/gl.h>
#include "eglport.h"
# include <GLES/gl.h>
# include <GLES/glext.h>
# ifndef APIENTRY
# define APIENTRY
# endif
#define GL_CLAMP                        GL_CLAMP_TO_EDGE
#define GL_MAX_TEXTURE_UNITS_ARB        GL_MAX_TEXTURE_IMAGE_UNITS
#define GL_MIRRORED_REPEAT_ARB          GL_MIRRORED_REPEAT_OES
#define GL_MIRRORED_REPEAT_IBM          GL_MIRRORED_REPEAT_OES
#define GL_TEXTURE0_ARB			GL_TEXTURE0
#define GL_TEXTURE1_ARB			GL_TEXTURE1
#define GL_TEXTURE2_ARB			GL_TEXTURE2
#define GL_TEXTURE3_ARB			GL_TEXTURE3
#define GL_TEXTURE4_ARB			GL_TEXTURE4
#define GL_TEXTURE5_ARB			GL_TEXTURE5
#define GL_TEXTURE6_ARB			GL_TEXTURE6
#define GL_TEXTURE7_ARB			GL_TEXTURE7
#define GL_ADD_SIGNED_ARB		GL_ADD_SIGNED
#define GL_SUBTRACT_ARB			GL_SUBTRACT
#define GL_INTERPOLATE_ARB		GL_INTERPOLATE
#define GL_CONSTANT_ARB			GL_CONSTANT
#define GL_PREVIOUS_ARB			GL_PREVIOUS
#define GL_COMBINE_ARB			GL_COMBINE
#define GL_COMBINE_RGB_ARB		GL_COMBINE_RGB
#define GL_COMBINE_ALPHA_ARB	GL_COMBINE_ALPHA
#define GL_PRIMARY_COLOR_ARB	GL_PRIMARY_COLOR
#define GL_SOURCE0_RGB_ARB		GL_SRC0_RGB
#define GL_OPERAND0_RGB_ARB		GL_OPERAND0_RGB
#define GL_SOURCE1_RGB_ARB		GL_SRC1_RGB
#define GL_OPERAND1_RGB_ARB		GL_OPERAND1_RGB
#define GL_SOURCE2_RGB_ARB		GL_SRC2_RGB
#define GL_OPERAND2_RGB_ARB		GL_OPERAND2_RGB
#define GL_SOURCE3_RGB_ARB		GL_SRC3_RGB
#define GL_OPERAND3_RGB_ARB		GL_OPERAND3_RGB
#define GL_SOURCE0_ALPHA_ARB		GL_SRC0_ALPHA
#define GL_OPERAND0_ALPHA_ARB		GL_OPERAND0_ALPHA
#define GL_SOURCE1_ALPHA_ARB		GL_SRC1_ALPHA
#define GL_OPERAND1_ALPHA_ARB		GL_OPERAND1_ALPHA
#define GL_SOURCE2_ALPHA_ARB		GL_SRC2_ALPHA
#define GL_OPERAND2_ALPHA_ARB		GL_OPERAND2_ALPHA
#define GL_SOURCE3_ALPHA_ARB		GL_SRC3_ALPHA
#define GL_OPERAND3_ALPHA_ARB		GL_OPERAND3_ALPHA
#define GL_MAX_TEXTURE_IMAGE_UNITS	GL_MAX_TEXTURE_UNITS

#define GL_RGBA8					GL_RGBA
#define	GL_RGB5_A1					0x8057 
#define GL_RGBA4					0x8056

#define glClearDepth                    glClearDepthf
#define glDepthRange					glDepthRangef
#define glOrtho							glOrthof

#define glColor4fv(a)						glColor4f(a[0], a[1], a[2], a[3])

#define glActiveTextureARB				glActiveTexture
#define glClientActiveTextureARB		glClientActiveTexture
#define APIENTRYP		GL_API
#define GLdouble		GLfloat

// save of GL Array
extern GLint	glsav_col_size;
extern GLenum	glsav_col_type;
extern GLsizei	glsav_col_stride;
extern GLvoid*	glsav_col_array;

extern GLint	glsav_vtx_size;
extern GLenum	glsav_vtx_type;
extern GLsizei	glsav_vtx_stride;
extern GLvoid*	glsav_vtx_array;

extern GLint	glsav_tex_size;
extern GLenum	glsav_tex_type;
extern GLsizei	glsav_tex_stride;
extern GLvoid*	glsav_tex_array;

extern GLint	glsav_tex1_size;
extern GLenum	glsav_tex1_type;
extern GLsizei	glsav_tex1_stride;
extern GLvoid*	glsav_tex1_array;

#else
#include <GL/gl.h>
#endif
#endif
#ifndef WIN32
    #include <GL/glext.h>
#endif

#endif
