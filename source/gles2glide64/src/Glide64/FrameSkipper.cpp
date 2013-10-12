/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Copyright (C) 2011 yongzh (freeman.yong@gmail.com)                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "FrameSkipper.h"
#include "ticks.h"
#include <stdio.h>

FrameSkipper::FrameSkipper()
	: skipType(AUTO), maxSkips(2), targetFPS(60)
{
}

void FrameSkipper::start()
{
	initialTicks = 0;
	virtualCount = 0;
	skipCounter = 0;
	oldSkip = 0;
}
#if 0
bool FrameSkipper::hasSkipped()
{
	oldSkip = skipCounter;
	
	if (skipType == MANUAL) {
		if (++skipCounter > maxSkips)
			skipCounter = 0;
//printf("frameskipper(Manual): oldSkip=%i, skipCounter=%i, countVI=%i\n", oldSkip, skipCounter, countVI);
		countVI = 0;
		return (oldSkip>0);
	}

	unsigned int elapsed = ticksGetTicks()/* - initialTicks*/;
	int realCount = (elapsed-initialTicks - 4) * targetFPS; // 4ms tolerance
	if ((realCount > countVI*1000) && (skipCounter < maxSkips) && (countVI<10)) {
		skipCounter++;
//printf("Skip frame elapsed=%u realCount=%u countVI=%u skipCounter=%i\n", elapsed-initialTicks, realCount, countVI, skipCounter);
	} else {
//printf(" Ok  frame elapsed=%u realCount=%u countVI=%u skipCounter=%i\n", elapsed-initialTicks, realCount, countVI, skipCounter);
		skipCounter = 0;
		initialTicks=elapsed;
		virtualCount=0;
		countVI=0;
	}
//printf("frameskipper(Auto): oldSkip=%i, skipCounter=%i, countVI=%i, realCount=%u\n", oldSkip, skipCounter, countVI, realCount);
	return (oldSkip>0);
}
#else
void FrameSkipper::newFrame()
{
	oldSkip = skipCounter;
	
	if (skipType == MANUAL) {
		if (++skipCounter > maxSkips)
			skipCounter = 0;
//printf("frameskipper(Manual): oldSkip=%i, skipCounter=%i, countVI=%i\n", oldSkip, skipCounter, countVI);
		countVI = 0;
		return;
	}

	unsigned int elapsed = ticksGetTicks()/* - initialTicks*/;
	int realCount = (elapsed-initialTicks - 4) * targetFPS; // 4ms tolerance
	if ((realCount > countVI*1000) && (skipCounter < maxSkips) && (countVI<10)) {
		skipCounter++;
//printf("Skip frame elapsed=%u realCount=%u countVI=%u skipCounter=%i\n", elapsed-initialTicks, realCount, countVI, skipCounter);
	} else {
//printf(" Ok  frame elapsed=%u realCount=%u countVI=%u skipCounter=%i\n", elapsed-initialTicks, realCount, countVI, skipCounter);
		skipCounter = 0;
		initialTicks=elapsed;
		virtualCount=0;
		countVI=0;
	}
//printf("frameskipper(Auto): oldSkip=%i, skipCounter=%i, countVI=%i, realCount=%u\n", oldSkip, skipCounter, countVI, realCount);
	return;
}
#endif

void FrameSkipper::update()
{
#if 1
	if (initialTicks == 0) {
		initialTicks = ticksGetTicks();
	}
	countVI++;
	
	if (countVI>20) {
		skipCounter=0;		// failsafe...
	}
#else
	// for the first frame
	if (initialTicks == 0) {
		initialTicks = ticksGetTicks();
		return;
	}
	
	oldSkip=skipCounter;
	
	if (skipType == MANUAL) {
		if (++skipCounter > maxSkips)
			skipCounter = 0;
		return;
	}

	unsigned int elapsed = ticksGetTicks()/* - initialTicks*/;
	unsigned int realCount = (elapsed-initialTicks) * targetFPS;

	virtualCount+=1000;
//	if (realCount >= virtualCount) {
		if (realCount > virtualCount+100 &&
				/*skipType == AUTO &&*/ skipCounter < maxSkips) {
			skipCounter++;
//printf("Skip frame elapsed=%u initialTicks=%u realCount=%u virtualCound=%u skipCounter=%i\n", elapsed, initialTicks, realCount, virtualCount, skipCounter);
		} else {
//			virtualCount = realCount;
//			if (skipType == AUTO)
				skipCounter = 0;
			initialTicks=elapsed;
			virtualCount=0;
		}
/*	} else {
		skipCounter = 0;
		initialTicks=elapsed;
		virtualCount=0;
	}*/
#endif
}
