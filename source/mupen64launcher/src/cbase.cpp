/**
 *  @section LICENSE
 *
 *  PickleLauncher
 *  Copyright (C) 2010-2011 Scott Smith
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @section LOCATION
 */

#include "cbase.h"

CBase::CBase()
{
}

CBase::~CBase()
{
}

void CBase::Log( const char* output, ... )
{
    va_list fmtargs;
    char buffer[1024];

    va_start( fmtargs, output );
    vsnprintf( buffer, sizeof(buffer)-1, output, fmtargs );
    va_end( fmtargs );

    fprintf( stdout, "%s", buffer );

#if defined(DEBUG)
    FILE* fout = fopen( "log.txt", "a" );
    if (!fout)
    {
        printf( "Failed to open logfile\n" );
        return;
    }
    fputs(buffer, fout);
    fclose(fout);
#endif
}

uint32_t CBase::CheckRange( int32_t value, int32_t size )
{
    if (value >= 0 && value < size)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint32_t CBase::rgb_to_int( SDL_Color color )
{
    return SDL_MapRGB(SDL_GetVideoSurface()->format, color.r, color.g, color.b);
}

int32_t CBase::a_to_i( const string& line )
{
    int32_t number;

    stringstream ss( line.c_str() );
    ss >> number;       // Convert string to integer
    return number;
}

string CBase::i_to_a( const int16_t num )
{
    string str;
    stringstream ss;

    ss << num;
    ss >> str;          // Convert string to integer
    return str;
}

string CBase::lowercase( string text )
{
    transform( text.begin(), text.end(), text.begin(), (int (*)(int))tolower );
    return text;
}

SDL_Surface* CBase::LoadImage( const string& filename )
{
    SDL_Surface* loaded_image    = NULL; // The mpImage that's loaded
    SDL_Surface* optimized_image = NULL; // The optimized surface that will be used

    loaded_image = IMG_Load( filename.c_str() ); //Load the mpImage

    // If the mpImage loaded
    if (NULL != loaded_image)
    {
        optimized_image = SDL_DisplayFormatAlpha( loaded_image ); // Create an optimized surface
        SDL_FreeSurface(loaded_image);                            // Free the old surface
        loaded_image = NULL;

        // If the surface was optimized
        if (NULL != optimized_image)
        {
            // Color key surface
            SDL_SetColorKey( optimized_image, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB( optimized_image->format, 0xFF, 0, 0xFF ) );
        }
    }
    else
    {
  		//Log( "LoadImage -> Could not load image: %s at path='%s'\n", IMG_GetError(), filename.c_str() );
  		return NULL;
    }

    //Return the optimized surface
    return optimized_image;
}

void CBase::ApplyImage( int16_t x, int16_t y, SDL_Surface* src, SDL_Surface* dst, SDL_Rect* clip )
{
    // Holds offsets
    SDL_Rect offset;

    // Get offsets
    offset.x = x;
    offset.y = y;

    // Blit
    SDL_BlitSurface( src, clip, dst, &offset );
}

bool CBase::CheckRectCollision( SDL_Rect* boxA, SDL_Rect* boxB )
{
    // The sides of the SDL_Rects
    int16_t leftA, leftB;
    int16_t rightA, rightB;
    int16_t topA, topB;
    int16_t bottomA, bottomB;

    // Calculate the sides of rec mCollisionbox
    leftA   = boxB->x;
    rightA  = boxB->x + boxB->w;
    topA    = boxB->y;
    bottomA = boxB->y + boxB->h;

    // Calculate the sides of rec box
    leftB   = boxA->x;
    rightB  = boxA->x + boxA->w;
    topB    = boxA->y;
    bottomB = boxA->y + boxA->h;

    // If any of the sides from mCollisionbox are outside of box
    if (bottomA <= topB) return false;
    if (topA >= bottomB) return false;
    if (rightA <= leftB) return false;
    if (leftA >= rightB) return false;
    // If none of the sides from mCollisionbox are outside box
    return true;    // Collision has occured
}

void CBase::SplitString( const std::string& delimiter, const std::string& text, vector<string>& array )
{
    string::size_type pos1, pos2;
    string value;

    array.clear();

    pos1 = text.find( delimiter, 0 );
    if (pos1 == string::npos)
    {
        pos1 = text.length();
    }
    value = text.substr( 0, pos1 );
    array.push_back(value);

    pos2 = pos1;

    do {
        pos1 = text.find( delimiter, pos2 );

        if (pos1 != string::npos)
        {
            pos2 = text.find( delimiter, pos1+1 );
            if (pos2 == string::npos)
            {
                pos2 = text.length();
            }

            value = text.substr( pos1+1, pos2-pos1-1 );
            array.push_back(value);
        }
    } while (pos1 != string::npos);
}

bool CBase::UnprefixString( string& result, const string& line, const string& prefix )
{
    string::size_type pos;

    pos = line.find(prefix, 0);
    if (pos == 0)
    {
        // Remove the prefix
        pos = line.find("=", 0) + 1;
        result = line.substr(pos, line.length()-pos);
        // Remove any comments
        pos = result.find("#", 0);
        result = result.substr(0, pos);
        // Trim left and right white spaces
        result.erase( result.begin(), std::find_if(result.begin(), result.end(), std::not1(std::ptr_fun<int, int>(std::isspace))) );
        result.erase( std::find_if(result.rbegin(), result.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), result.end() );

        return true;
    }
    return false;
}

int16_t CBase::CheckExtension( const string& filename, const string& ext )
{
    int16_t result;
    string::size_type pos;

    pos = filename.length() - ext.length();
    result = lowercase(filename).find( lowercase(ext), pos);

    return result;
}

void CBase::CheckPath( string& path )
{
    if (path.length()>1)
    {
        if (path.at(path.length()-1) != '/')
        {
            path += '/';
        }
    }
    else
    {
        Log( "Warning: CheckPath path too short\n" );
    }
}

string CBase::cmdclean( string& cmdline )
{
    string spchars = "\\`$();|{}&'\"*?<>[]!^~-#\n\r ";
    for (uint i=0; i<spchars.length(); i++)
    {
        string curchar = spchars.substr(i,1);
        cmdline = strreplace(cmdline, curchar, "\\"+curchar);
    }
    return cmdline;
}

string CBase::strreplace( string& orig, const string& search, const string& replace )
{
    string::size_type pos = orig.find( search, 0 );
    while (pos != string::npos)
    {
        orig.replace(pos,search.length(),replace);
        pos = orig.find( search, pos+replace.length() );
    }
    return orig;
}

SDL_Surface* CBase::ScaleSurface( SDL_Surface *surface, uint16_t width, uint16_t height )
{
    if(!surface || !width || !height)
        return 0;

    SDL_Surface *_ret = SDL_CreateRGBSurface(surface->flags, width, height, surface->format->BitsPerPixel,
        surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

    double  _stretch_factor_x = ( static_cast<double>(width) / static_cast<double>(surface->w) ),
        _stretch_factor_y = ( static_cast<double>(height) / static_cast<double>(surface->h) );

    for (int32_t y = 0; y < surface->h; y++)
        for (int32_t x = 0; x < surface->w; x++)
            for (int32_t o_y = 0; o_y < _stretch_factor_y; ++o_y)
                for (int32_t o_x = 0; o_x < _stretch_factor_x; ++o_x)
                    putpixel( _ret, static_cast<int32_t>(_stretch_factor_x * x) + o_x,
                        static_cast<int32_t>(_stretch_factor_y * y) + o_y, getpixel(surface, x, y) );

    return _ret;
}

uint32_t CBase::getpixel( SDL_Surface *surface, int16_t x, int16_t y )
{
    int16_t bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
            break;

        case 2:
            return *(uint16_t *)p;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(uint32_t *)p;
            break;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void CBase::putpixel( SDL_Surface *surface, int16_t x, int16_t y, uint32_t pixel )
{
    int16_t bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(uint16_t *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
        default:
            *(uint32_t *)p = pixel;
            break;
    }
}
