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

 /*
   czip.cpp is based on miniunz.c

   miniunz.c
   Version 1.1, February 14h, 2010
   sample part of the MiniZip project - ( http://www.winimage.com/zLibDll/minizip.html )

         Copyright (C) 1998-2010 Gilles Vollant (minizip) ( http://www.winimage.com/zLibDll/minizip.html )

         Modifications of Unzip for Zip64
         Copyright (C) 2007-2008 Even Rouault

         Modifications for Zip64 support on both zip and unzip
         Copyright (C) 2009-2010 Mathias Svensson ( http://result42.com )
*/

#include "czip.h"

CZip::CZip() : CBase(),
    UnzipFiles          ()
{
}

CZip::~CZip()
{
}

void CZip::ListFiles( const string& zipfile, vector<string>& list )
{
    uint32_t i;
    int32_t err;
    unzFile uf=NULL;
    unz_global_info64 gi;

    // Open the zip file
    uf = unzOpen64( zipfile.c_str() );

    if (uf != NULL)
    {
        // Get file info for the zip file
        err = unzGetGlobalInfo64( uf,&gi );
        if (err != UNZ_OK)
        {
            Log( "error %d with zipfile in unzGetGlobalInfo\n",err);
            return;
        }
    }
    else
    {
        Log( "error with zipfile %s in unzOpen64\n", zipfile.c_str() );
        return;
    }
    Log( "Reading zip file %s\n", zipfile.c_str() );

    // Spit out some information about the files in the zip for debug
    Log( "  Length  Method     Size Ratio   Date    Time   CRC-32     Name\n" );
    Log( "  ------  ------     ---- -----   ----    ----   ------     ----\n" );
    for (i=0; i<gi.number_entry; i++)
    {
        char filename_inzip[256];
        unz_file_info64 file_info;
        uLong ratio=0;
        const char *string_method = NULL;
        char charCrypt=' ';
        err = unzGetCurrentFileInfo64( uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0 );
        if (err != UNZ_OK)
        {
            Log( "error %d with zipfile in unzGetCurrentFileInfo\n",err);
            break;
        }
        if (file_info.uncompressed_size > 0)
            ratio = (uLong)((file_info.compressed_size*100)/file_info.uncompressed_size);

        /* display a '*' if the file is crypted */
        if ((file_info.flag & 1) != 0)
            charCrypt='*';

        if (file_info.compression_method == 0)
            string_method="Stored";
        else
        if (file_info.compression_method == Z_DEFLATED)
        {
            uInt iLevel=(uInt)((file_info.flag & 0x6)/2);
            if (iLevel==0)
              string_method="Defl:N";
            else if (iLevel==1)
              string_method="Defl:X";
            else if ((iLevel==2) || (iLevel==3))
              string_method="Defl:F"; /* 2:fast , 3 : extra fast*/
        }
        else
        if (file_info.compression_method == Z_BZIP2ED)
        {
              string_method="BZip2 ";
        }
        else
            string_method="Unkn. ";

        Display64BitsSize( file_info.uncompressed_size,7 );
        Log( "  %6s%c",string_method,charCrypt);
        Display64BitsSize( file_info.compressed_size,7 );
        Log( " %3lu%%  %2.2lu-%2.2lu-%2.2lu  %2.2lu:%2.2lu  %8.8lx   %s\n",
                ratio,
                (uLong)file_info.tmu_date.tm_mon + 1,
                (uLong)file_info.tmu_date.tm_mday,
                (uLong)file_info.tmu_date.tm_year % 100,
                (uLong)file_info.tmu_date.tm_hour,(uLong)file_info.tmu_date.tm_min,
                (uLong)file_info.crc,filename_inzip);
        if ((i+1) < gi.number_entry)
        {
            err = unzGoToNextFile( uf );
            if (err != UNZ_OK)
            {
                Log( "error %d with zipfile in unzGoToNextFile\n",err);
                break;
            }
        }

        // Save the names of the files in the zip
        list.push_back( filename_inzip );
    }

    // Close the zip file
    unzClose( uf );
}

void CZip::ExtractFile( const string& zipfile, const string& location, const string& filename )
{
    uint32_t i;
    int32_t err;
    unzFile uf = NULL;
    unz_global_info64 gi;

    // Open the zip file
    uf = unzOpen64( zipfile.c_str() );

    if (uf != NULL)
    {
        // Get file info for the zip file
        err = unzGetGlobalInfo64( uf,&gi );
        if (err != UNZ_OK)
        {
            Log( "error %d with zipfile in unzGetGlobalInfo\n",err);
            return;
        }
    }
    else
    {
        Log( "error with zipfile %s in unzOpen64\n", zipfile.c_str() );
        return;
    }

    for (i=0; i<gi.number_entry; i++)
    {
        char filename_inzip[256];
        unz_file_info64 file_info;

        err = unzGetCurrentFileInfo64( uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0 );
        if (err != UNZ_OK)
        {
            Log( "error %d with zipfile in unzGetCurrentFileInfo\n",err);
            break;
        }

        if (filename.compare(filename_inzip) == 0)
        {
            Extract( uf, location );
            break;
        }

        if ((i+1) < gi.number_entry)
        {
            err = unzGoToNextFile( uf );
            if (err != UNZ_OK)
            {
                Log( "error %d with zipfile in unzGoToNextFile\n",err);
                break;
            }
        }
    }

    // Close the zip file
    unzClose( uf );
}

void CZip::ExtractFiles( const string& zipfile, const string& location )
{
    uint16_t i;
    int32_t err;
    unzFile uf=NULL;
    unz_global_info64 gi;

    uf = unzOpen64( zipfile.c_str() );

    if (uf != NULL)
    {
        err = unzGetGlobalInfo64( uf, &gi );
        if (err != UNZ_OK)
        {
            Log( "error %d with zipfile in unzGetGlobalInfo \n", err );
            return;
        }
    }
    else
    {
        Log( "error with zipfile in unzOpen64 \n" );
        return;
    }

    for (i=0;i<gi.number_entry;i++)
    {
        if (Extract( uf, location ) != UNZ_OK)
        {
            break;
        }

        if ((uint16_t)(i+1) < gi.number_entry)
        {
            // Go the next file
            err = unzGoToNextFile( uf );
            if (err != UNZ_OK)
            {
                Log( "error %d with zipfile in unzGoToNextFile\n", err );
                break;
            }
        }
    }

    // Close the zip file
    unzClose( uf );
}

#define WRITEBUFFERSIZE (8192)
int32_t CZip::Extract( unzFile uf, const string& location )
{
    string write_filename;
    void*   buf;
    char    filename_inzip[256];
    int32_t err=UNZ_OK;
    FILE*   fout=NULL;
    unz_file_info64 file_info;

    err = unzGetCurrentFileInfo64( uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0 );
    if (err != UNZ_OK)
    {
        Log( "error %d with zipfile in unzGetCurrentFileInfo\n", err );
        return err;
    }

    buf = (void*)malloc(WRITEBUFFERSIZE);
    if (buf == NULL)
    {
        Log( "Error allocating memory\n" );
        free(buf);
        return UNZ_INTERNALERROR;
    }

    err = unzOpenCurrentFile( uf );
    if (err != UNZ_OK)
    {
        Log( "error %d with zipfile in unzOpenCurrentFile\n", err );
        free(buf);
        return err;
    }

    write_filename = location + '/' + string(filename_inzip);

    fout = fopen64( write_filename.c_str(), "wb" );

    if (fout != NULL)
    {
        Log( " extracting: %s\n", write_filename.c_str() );

        do {
            err = unzReadCurrentFile( uf, buf, WRITEBUFFERSIZE );
            if (err < 0)
            {
                Log( "error %d with zipfile in unzReadCurrentFile\n",err);
                break;
            }
            if (err > 0)
            {
                if (fwrite( buf, err, 1, fout ) != 1)
                {
                    Log( "error in writing extracted file\n" );
                    err = UNZ_ERRNO;
                    break;
                }
            }
        }
        while (err > 0);

        if (fout)
        {
            fclose( fout );
        }
    }

    if (err==UNZ_OK)
    {
        err = unzCloseCurrentFile( uf );
        if (err != UNZ_OK)
        {
            Log( "error %d with zipfile in unzCloseCurrentFile\n",err);
        }

        AddUnzipFile( write_filename );
    }
    else
    {
        unzCloseCurrentFile( uf ); /* don't lose the error */
    }

    free(buf);
    return err;
}

void CZip::Display64BitsSize(ZPOS64_T n, int size_char)
{
  /* to avoid compatibility problem , we do here the conversion */
  char number[21];
  int offset=19;
  int pos_string = 19;
  number[20]=0;
  for (;;) {
      number[offset]=(char)((n%10)+'0');
      if (number[offset] != '0')
          pos_string=offset;
      n/=10;
      if (offset==0)
          break;
      offset--;
  }
  {
      int size_display_string = 19-pos_string;
      while (size_char > size_display_string)
      {
          size_char--;
          Log( " " );
      }
  }

  Log( "%s",&number[pos_string]);
}

void CZip::AddUnzipFile( const string& filename )
{
    uint16_t i;

    for (i=0; i<UnzipFiles.size(); i++)
    {
        if (UnzipFiles.at(i) == filename)
            return;
    }
    UnzipFiles.push_back( filename );
}

void CZip::DelUnzipFiles( void )
{
    uint16_t i;

    for (i=0; i<UnzipFiles.size(); i++)
    {
#if defined(DEBUG)
        Log( "Removing file %s\n", UnzipFiles.at(i).c_str() );
#endif
        remove( UnzipFiles.at(i).c_str() );
    }
    UnzipFiles.clear();
}

int8_t CZip::SaveUnzipList( const string& location )
{
    uint8_t i;
    ofstream   fout;

    fout.open(location.c_str(), ios_base::trunc);

    if (!fout)
    {
        Log( "Failed to open ziplist at %s\n", location.c_str() );
        return 1;
    }

    // Write out the profile
    if (fout.is_open())
    {
        for (i=0; i<UnzipFiles.size(); i++)
        {
            if (UnzipFiles.at(i).length()>0)
            {
#if defined(DEBUG)
                Log( "Saving file %s to ziplist\n", UnzipFiles.at(i).c_str());
#endif
                fout << UnzipFiles.at(i) << endl;
            }
        }
    }

    return 0;
}

int8_t CZip::LoadUnzipList( const string& location )
{
    string          line;
    ifstream        fin;

    fin.open(location.c_str(), ios_base::in);

    if (!fin)
    {
        Log( "Failed to open unziplist at %s\n", location.c_str() );
        return 0;   // Dont stop the app if it cant be opened, default values will be used and then save to file.
    }

    UnzipFiles.clear();

    // Read in the profile
    if (fin.is_open())
    {
        while (!fin.eof())
        {
            getline(fin,line);

            if (line.length() > 0)
            {
                UnzipFiles.push_back(line);
            }
        }
    }

    return 0;
}

