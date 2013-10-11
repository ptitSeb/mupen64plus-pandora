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

#include "csystem.h"

CSystem::CSystem() : CBase()
{
#if defined(GP2X) || defined(WIZ) || defined(CAANOO)
    memdev = open( "/dev/mem", O_RDWR );
    if (memdev == 0)
    {
        Log( "Could not open /dev/mem\n" );
    }
    else
    {
        memregs = (uint32_t*)mmap( 0, MMAP_ADDRESS, PROT_READ|PROT_WRITE, MAP_SHARED, memdev, 0xc0000000 );

        if (memregs == MAP_FAILED)
        {
            Log( "Could not mmap hardware registers!\n" );
            close(memdev);
        }
    }
#endif
}

CSystem::~CSystem()
{
#if defined(GP2X) || defined(WIZ) || defined(CAANOO)
    if (memdev != 0)
    {
        memregs = NULL;
    }
    close(memdev);
#endif
}

void CSystem::SetCPUClock( uint16_t& mhz )
{
    // Range check
    if (mhz == 0 || mhz > CPU_CLOCK_MAX)
    {
        Log( "CPU mhz out of range, resetting to default. Value is now %d and allowed values should be between 0 and %d Mhz.\n", mhz, CPU_CLOCK_MAX );
        mhz = CPU_CLOCK_DEF;
    }

#if defined(PANDORA)
    string command = "/usr/bin/sudo cpuset " + i_to_a(mhz);
    execlp( command.c_str(), command.c_str(), NULL, NULL, NULL );

#elif defined(WIZ) || defined(CAANOO)
    if (memdev != 0 && memregs != 0)
    {
        volatile uint32_t *memregl = static_cast<volatile uint32_t*>((volatile void*)memregs);
        uint32_t mdiv, pdiv = 9, sdiv = 0;
        uint32_t v;

        mdiv = (mhz * pdiv) / SYS_CLK_FREQ;
        if (mdiv & ~0x3ff) return;
        v = pdiv<<18 | mdiv<<8 | sdiv;

        PLLSETREG0 = v;
        PWRMODE |= 0x8000;
        for (int i = 0; (PWRMODE & 0x8000) && i < 0x100000; i++);
    }

#elif defined(GP2X)
    if (memdev != 0 && memregs != 0)
    {
        uint32_t v;
        uint32_t mdiv, pdiv=3, scale=0;

        mhz *= 1000000;
        mdiv = (mhz * pdiv) / SYS_CLK_FREQ;
        mdiv = ((mdiv-8)<<8) & 0xff00;
        pdiv = ((pdiv-2)<<2) & 0xfc;
        scale &= 3;
        v = mdiv | pdiv | scale;
        MEM_REG[0x910>>1] = v;
    }

#else
    Log( "Setting CPU Clock not supported on this machine.\n" );
#endif
}
