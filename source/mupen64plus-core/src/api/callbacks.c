/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus-core - api/callbacks.c                                    *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2009 Richard Goedeken                                   *
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

/* This file contains the Core functions for handling callbacks to the
 * front-end application
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "m64p_types.h"
#include "callbacks.h"

/* local variables */
static ptr_DebugCallback pDebugFunc = NULL;
static ptr_StateCallback pStateFunc = NULL;
static void *            DebugContext = NULL;
static void *            StateContext = NULL;

#if defined(DEBUG_MSG_TO_LOGFILE)
time_t rawtime;
struct tm * timeinfo;
char buffer[10];
extern FILE *pDebugFile;
#endif
 
 /* global Functions for use by the Core */
m64p_error SetDebugCallback(ptr_DebugCallback pFunc, void *Context)
{
    pDebugFunc = pFunc;
    DebugContext = Context;
    return M64ERR_SUCCESS;
}

m64p_error SetStateCallback(ptr_StateCallback pFunc, void *Context)
{
    pStateFunc = pFunc;
    StateContext = Context;
    return M64ERR_SUCCESS;
}

void DebugMessage(int level, const char *message, ...)
{
  char msgbuf[256];
  va_list args;

  if (pDebugFunc == NULL)
      return;

#if defined(DEBUG_MSG_TO_LOGFILE)
  if(pDebugFile != NULL)
  {
   time (&rawtime);
   timeinfo = (struct tm *)localtime (&rawtime);
   strftime (buffer,10,"%H:%M:%S",timeinfo);
      fprintf (pDebugFile, "%s: ", buffer);
  }
#endif

  va_start(args, message);
  vsprintf(msgbuf, message, args);

#if defined(DEBUG_MSG_TO_LOGFILE)
  if(pDebugFile != NULL)
    vfprintf(pDebugFile,message,args);
#endif

  (*pDebugFunc)(DebugContext, level, msgbuf);

  va_end(args);
#if defined(DEBUG_MSG_TO_LOGFILE)
  if(pDebugFile != NULL)
  {
    fprintf (pDebugFile, "\n");
    fflush(pDebugFile);
  }
#endif
}

void StateChanged(m64p_core_param param_type, int new_value)
{
    if (pStateFunc == NULL)
        return;

    (*pStateFunc)(StateContext, param_type, new_value);
}


