/*
Copyright (C) 2003 Rice1964

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <stdlib.h>

#include "Config.h"
#include "Debugger.h"
#include "OGLDebug.h"
#include "OGLGraphicsContext.h"
#include "OGLTexture.h"
#include "TextureManager.h"

#ifdef HAVE_GLES
#define GL_RGBA4                          0x8056
#endif

COGLTexture::COGLTexture(uint32 dwWidth, uint32 dwHeight, TextureUsage usage) :
    CTexture(dwWidth,dwHeight,usage),
    m_glFmt(GL_RGBA)
{
    // FIXME: If usage is AS_RENDER_TARGET, we need to create pbuffer instead of regular texture

    m_dwTextureFmt = TEXTURE_FMT_A8R8G8B8;  // Always use 32bit to load texture
    glGenTextures( 1, &m_dwTextureName );
    OPENGL_CHECK_ERRORS;

    // Make the width and height be the power of 2
    uint32 w;
    for (w = 1; w < dwWidth; w <<= 1);
    m_dwCreatedTextureWidth = w;
    for (w = 1; w < dwHeight; w <<= 1);
    m_dwCreatedTextureHeight = w;
    
    if (dwWidth*dwHeight > 256*256)
        TRACE4("Large texture: (%d x %d), created as (%d x %d)", 
            dwWidth, dwHeight,m_dwCreatedTextureWidth,m_dwCreatedTextureHeight);
    
    m_fYScale = (float)m_dwCreatedTextureHeight/(float)m_dwHeight;
    m_fXScale = (float)m_dwCreatedTextureWidth/(float)m_dwWidth;

    m_pTexture = malloc(m_dwCreatedTextureWidth * m_dwCreatedTextureHeight * GetPixelSize());
//#ifndef HAVE_GLES

    switch( options.textureQuality )
    {
    case TXT_QUALITY_DEFAULT:
        if( options.colorQuality == TEXTURE_FMT_A4R4G4B4 ) 
            m_glFmt = GL_RGBA4;
        break;
    case TXT_QUALITY_32BIT:
        break;
    case TXT_QUALITY_16BIT:
            m_glFmt = GL_RGBA4;
        break;
    };

//#endif
    LOG_TEXTURE(TRACE2("New texture: (%d, %d)", dwWidth, dwHeight));
}

COGLTexture::~COGLTexture()
{
    // FIXME: If usage is AS_RENDER_TARGET, we need to destroy the pbuffer

    glDeleteTextures(1, &m_dwTextureName );
    OPENGL_CHECK_ERRORS;
    free(m_pTexture);
    m_pTexture = NULL;
    m_dwWidth = 0;
    m_dwHeight = 0;
}

bool COGLTexture::StartUpdate(DrawInfo *di)
{
    if (m_pTexture == NULL)
        return false;

    di->dwHeight = (uint16)m_dwHeight;
    di->dwWidth = (uint16)m_dwWidth;
    di->dwCreatedHeight = m_dwCreatedTextureHeight;
    di->dwCreatedWidth = m_dwCreatedTextureWidth;
    di->lpSurface = m_pTexture;
    di->lPitch = GetPixelSize()*m_dwCreatedTextureWidth;

    return true;
}

void COGLTexture::EndUpdate(DrawInfo *di)
{
    COGLGraphicsContext *pcontext = (COGLGraphicsContext *)(CGraphicsContext::g_pGraphicsContext); // we need this to check if the GL extension is avaible

    glBindTexture(GL_TEXTURE_2D, m_dwTextureName);
    OPENGL_CHECK_ERRORS;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    OPENGL_CHECK_ERRORS;

    // mipmap support
    if(options.mipmapping)
    {
        int m_maximumAnistropy = pcontext->getMaxAnisotropicFiltering(); //if getMaxAnisotropicFiltering() return more than 0, so aniso is supported and maxAnisotropicFiltering is set

        // Set Anisotropic filtering (mipmapping have to be activated, aniso filtering is not effective without)
        if( m_maximumAnistropy )
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_maximumAnistropy);
            OPENGL_CHECK_ERRORS;
        }

        // Set Mipmap
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        OPENGL_CHECK_ERRORS;

#if SDL_VIDEO_OPENGL
        // Tell to hardware to generate mipmap (himself) when glTexImage2D is called
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
#elif SDL_VIDEO_OPENGL_ES2
        glGenerateMipmap(GL_TEXTURE_2D);
#endif
        OPENGL_CHECK_ERRORS;
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        OPENGL_CHECK_ERRORS;
    }

    // Copy the image data from main memory to video card texture memory
#if SDL_VIDEO_OPENGL
#ifdef HAVE_GLES
	// first change format BGRA->RGBA and reduce bit if needed
	unsigned char*  m_glesTex;
	if (m_glFmt == GL_RGBA4) {
		m_glesTex = (unsigned char*)malloc(m_dwCreatedTextureHeight*m_dwCreatedTextureWidth*2);
		unsigned short *p = (unsigned short*)m_glesTex;
		unsigned char *f = (unsigned char*)m_pTexture;
		for (int j=0; j<m_dwCreatedTextureHeight; j++)
			for (int i=0; i<m_dwCreatedTextureWidth; i++) {
				// *f = B, *(f+1) = G, *(f+2) = R, *(f+3) = A
				// RRRR GGGG BBBB AAAA
				*(p++)= ((*(f+2))&0xf0)>>4 | ((*(f+1))&0xf0) | (((*f))&0xf0)<<4 | ((*(f+3))&0xf0)<<8;
				f+=4;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_dwCreatedTextureWidth, m_dwCreatedTextureHeight, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, m_glesTex);
	} else {
		m_glesTex = (unsigned char*)malloc(m_dwCreatedTextureHeight*m_dwCreatedTextureWidth*4);
		unsigned int *p = (unsigned int*)m_glesTex;
		unsigned char *f = (unsigned char*)m_pTexture;
		for (int j=0; j<m_dwCreatedTextureHeight; j++)
			for (int i=0; i<m_dwCreatedTextureWidth; i++) {
				// *f = B, *(f+1) = G, *(f+2) = R, *(f+3) = A
				// AAAAAAAA BBBBBBBB GGGGGGGG RRRRRRRR
				*(p++)= ((*(f+2))) | ((*(f+1)))<<8 | ((*f))<<16 | ((*(f+3)))<<24;
				f+=4;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_dwCreatedTextureWidth, m_dwCreatedTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_glesTex);
	}
	free(m_glesTex);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, m_glFmt, m_dwCreatedTextureWidth, m_dwCreatedTextureHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, m_pTexture);
#endif
#elif SDL_VIDEO_OPENGL_ES2
    //GL_BGRA_IMG works on adreno but not inside profiler.
    glTexImage2D(GL_TEXTURE_2D, 0, m_glFmt, m_dwCreatedTextureWidth, m_dwCreatedTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pTexture);
#endif
    OPENGL_CHECK_ERRORS;
}


// Keep in mind that the real texture is not scaled to fix the created opengl texture yet.
// when the image is need to be scaled, ScaleImageToSurface in CTexure will be called to 
// scale the image automatically

