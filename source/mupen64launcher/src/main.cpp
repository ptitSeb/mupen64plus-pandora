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

#include "main.h"

int32_t main( int32_t argc, char** argv )
{
    int32_t result;
    CSelector selector;

    selector.Log( "Starting %s Version %s.\n", APPNAME, APPVERSION );
    result = selector.Run( argc, argv );
    selector.Log( "Quitting %s Version %s.\n", APPNAME, APPVERSION );

    return result;
}
