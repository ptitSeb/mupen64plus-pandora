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

#ifndef CSYSTEM_H
#define CSYSTEM_H

#include "cbase.h"

using namespace std;

#if defined(PANDORA)
#define CPU_CLOCK_DEF       600
#define CPU_CLOCK_MAX      1200

#elif defined(WIZ) || defined(CAANOO)
#define CPU_CLOCK_DEF       300
#define CPU_CLOCK_MAX       600
#define MMAP_ADDRESS        0x10000
#define SYS_CLK_FREQ        27
#define PLLSETREG0          (memregl[0xF004>>2])
#define PWRMODE             (memregl[0xF07C>>2])

#elif defined(GP2X)
#define CPU_CLOCK_DEF       150
#define CPU_CLOCK_MAX       300
#define MMAP_ADDRESS        0x20000
#define SYS_CLK_FREQ        7372800

#elif defined(GP2X)
#define CPU_CLOCK_DEF       1000
#define CPU_CLOCK_MAX       2000

#else
#define CPU_CLOCK_DEF       1
#define CPU_CLOCK_MAX       1
#endif

/** @brief This class controls resources, logic for gui, interaction with the user.
 */
class CSystem : public CBase
{
    public:
        /** Constructor. */
        CSystem();
        /** Destructor. */
        virtual ~CSystem();

        /** @brief Set the CPU clock of the system.
         * @param clock : number of arguments.
         * @return 0 if passed 1 if failed.
         */
        void SetCPUClock( uint16_t& mhz );

    private:

#if defined(GP2X) || defined(WIZ) || defined(CAANOO)
        uint32_t memdev;
        volatile uint32_t *memregs;
#endif
};

#endif // CSYSTEM_H
