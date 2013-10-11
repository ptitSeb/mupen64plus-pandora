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

#ifndef MAIN_H
#define MAIN_H

#include "version.h"
#include "cselector.h"

/**< Defines. */
/**<    DEBUG               : enables additional code and logging for debug purposes. */
/**<    DEBUG_FPS           : prints frame count information to the console. */
/**<    DEBUG_FORCE_REDRAW  : force the drawing routines to redraw to the screen every loop. */
/**<    DEBUG_DRAW_STATES   : prints the draw states every loop to the console. */
/**<    Platforms   : Select only one. */
/**<        PANDORA : enables specific settings for the pandora platform. */
/**<        CAANOO  : enables specific settings for the caanoo platform. */
/**<        WIZ     : enables specific settings for the wiz platform. */
/**<        GP2X    : enables specific settings for the gp2x platform. */
/**<        X86     : enables specific settings for the pc platform. */

/** @brief Main entry into the application
 * @param argc : number of arguments
 * @param argv : arguments passed to the application
 * @return 0 if passed 1 if failed
 */
int32_t main( int32_t argc, char** argv );

#endif // MAIN_H
