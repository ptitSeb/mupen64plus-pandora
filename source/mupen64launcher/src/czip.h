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

#ifndef CZIP_H
#define CZIP_H

#include "cbase.h"
#include "unzip/unzip.h"

using namespace std;

/** @brief This class handles interfaces to read and extract files from zips.
 */
class CZip : public CBase
{
    public:
        /** Constructor. */
        CZip();
        /** Destructor. */
        virtual ~CZip();

        /** @brief Loads a list of names for files detected in the zip.
         * @param zipfile : the zip file to read.
         * @param list : the list of filenames.
         */
        void    ListFiles           ( const string& zipfile, vector<string>& list );

        /** @brief Extracts file within a zip to the designated location.
         * @param zipfile : the zip file to extract from.
         * @param location : location to extract the file to.
         * @param filename : the file to extract.
         * @param list : the list of filenames.
         */
        void    ExtractFile        ( const string& zipfile, const string& location, const string& filename );

        /** @brief Extracts files within a zip to the designated location.
         * @param zipfile : the zip file to extract from.
         * @param location : location to extract the files to.
         * @param list : the list of filenames.
         */
        void    ExtractFiles        ( const string& zipfile, const string& location );

        /** @brief Deletes files extracted at the designated location
         */
        void    DelUnzipFiles       ( void );

        /** @brief Save a list of extracted files.
         * @param location : where to save the zip list.
         * @return 0 if passed 1 if failed.
         */
        int8_t  SaveUnzipList       ( const string& location );

        /** @brief Load a list of extracted files.
         * @param location : where to load the zip list.
         * @return 0 if passed 1 if failed.
         */
        int8_t  LoadUnzipList       ( const string& location );

    private:
        /** @brief Display the zip size.
         * @param n : file size.
         * @param size_char : number of digits to display.
         */
        void    Display64BitsSize   ( ZPOS64_T n, int size_char);

        /** @brief Extracts a file within a zip to the designated location
         * @param location : location to extract the file to.
         * @return zip result.
         */
        int32_t Extract             ( unzFile uf, const string& location );

        /** @brief Stores the name of an extracted file to a list in memory.
         * @param filename : filename to record.
         */
        void    AddUnzipFile        ( const string& filename );

        vector<string>  UnzipFiles; /**< A list of filenames for files that have been extracted. */
};

#endif // CZIP_H
