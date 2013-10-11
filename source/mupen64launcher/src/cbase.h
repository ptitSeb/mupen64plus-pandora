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

#ifndef CBASE_H
#define CBASE_H

#include <cctype>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>	//*SEB* for execlp

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

using namespace std;

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))    /**< Return minimum of two numbers. */
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))    /**< Return maximum of two numbers. */

/** @brief Generic class to hold any common methods usable by any other class
 */
class CBase
{
    public:
        /** Constructor. */
        CBase();
        /** Destructor. */
        virtual ~CBase();

        /** @brief Logging function
         * @param output : formating string to write to the log
         */
        void            Log                 ( const char* output, ... );

        /** @brief Checks a value is between 0 and size
         * @param value : value for the range check
         * @param size : max that the value should be less than
         */
        uint32_t        CheckRange          ( int32_t value, int32_t size );

        /** @brief Convert color componets to a single integer
         * @param color : SDL color struct with color components
         * @return color in integer form
         */
        uint32_t        rgb_to_int          ( SDL_Color color );

        /** @brief Convert string to integer form
         * @param line : string to convert (typically one character)
         * @return string in integer form
         */
        int32_t         a_to_i              ( const string& line );

        /** @brief Convert integer to string form
         * @param num : integer to convert
         * @return integer in string form
         */
        string          i_to_a              ( const int16_t num );

        /** @brief Convert string to all lowercase characters
         * @param text : string to convert
         * @return copy fo string in all lowercase characters
         */
        string          lowercase           ( string text );

        /** @brief Load graphics from file
         * @param filename : file location to the graphic
         * @return pointer to the optimized graphic
         */
        SDL_Surface*    LoadImage           ( const string& filename );

        /** @brief Blit a surface to another surface(screen)
         * @param x : x coordinate location for the blit
         * @param y : y coordinate location for the blit
         * @param src : source graphic to blit to destination
         * @param dst : destination graphic to blit to source
         * @param clip : dimensions to clip the source graphic
         */
        void            ApplyImage          ( int16_t x, int16_t y, SDL_Surface* src, SDL_Surface* dst, SDL_Rect* clip );

        /** @brief Detect collision by determining if two rects overlap
         * @param boxA : first rect
         * @param boxB : second rect
         * @return true if collision occured, false if it has not
         */
        bool            CheckRectCollision  ( SDL_Rect* boxA, SDL_Rect* boxB );

        /** @brief Separate string into different substring based on a delimter string
         * @param delimiter : string value to separate text string
         * @param text : string to separate
         * @param array : contains all of the separate string parts
         */
        void            SplitString         ( const std::string& delimiter, const std::string& text, vector<string>& array );

        /** @brief Removes a prefix from the beginning of the line and returns the result
         * @param result : resulting string
         * @param line : string to separate
         * @param prefix : string to remove
         * @return true if prefix was found and removed, false if it has not
         */
        bool            UnprefixString      ( string& result, const string& line, const string& prefix );

        /** @brief Checks a string filename to be ended by a extension string
         * @param filename : string filename to scan
         * @param ext : string extension to find
         * @return position of ext if found
         */
        int16_t         CheckExtension      ( const string& filename, const string& ext );

        /** @brief Checks a string path if ended with a backslash
         * @param path : string path to scan
         */
        void            CheckPath           ( string& path );

        /** @brief Removes a list of characters from the string command line
         * @param cmdline : string command line to filter
         * @return the filtered string
         */
        string          cmdclean            ( string& cmdline );

        /** @brief Replaces one string for another string
         * @param orig : original string to search
         * @param search : string to search
         * @param replace : string to replace any instances of string search
         * @return the modified result
         */
        string          strreplace          ( string& orig, const string& search, const string& replace );

        /** @brief Scales the input surface to a new surface with the dimensions provided
         * @param surface : input image to be scaled
         * @param width : width of the new scaled image
         * @param height : height of the new scaled image
         * @return the new scaled image
         */
        SDL_Surface*    ScaleSurface        ( SDL_Surface *surface, uint16_t width, uint16_t height );

        /** @brief Gets the pixel data at the provided coordinates
         * @param surface : input image to be read
         * @param x : x position of the pixel
         * @param y : y position of the pixel
         * @return pixel data
         */
        uint32_t        getpixel            ( SDL_Surface *surface, int16_t x, int16_t y );

        /** @brief Writes the pixel data at the provided coordinates
         * @param surface : input image to be modified
         * @param x : x position of the pixel
         * @param y : y position of the pixel
         */
        void            putpixel            ( SDL_Surface *surface, int16_t x, int16_t y, uint32_t pixel );
};

#endif // CBASE_H
