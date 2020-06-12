#/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# *   Mupen64plus - Makefile                                                *
# *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
# *   Copyright (C)                                                         *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# include pre-make file with a bunch of definitions
#USES_GTK2 = true
#include ../pre.mk
CFLAGS += -I ../mupen64plus-core/src/api/

# local CFLAGS, LIBS, and LDFLAGS
ifneq "$(CPU)" "ARM"
CFLAGS += -fpic -DPIC
endif
CFLAGS += -Wall -ggdb -gdwarf-2
ifeq ($(ODROID),1)
 CFLAGS += -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=hard -ftree-vectorize -ffast-math
else
ifeq ($(PYRA),1)
 CFLAGS += -mcpu=cortex-a15 -mfpu=neon-vfpv4 -mfloat-abi=hard -ftree-vectorize -ffast-math -marm
else
 CFLAGS += -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -ftree-vectorize -ffast-math
endif
endif
CFLAGS += -O3

LDFLAGS += -ldl

TARGET = notaz_audio.so
OBJECTS = main.o osal_dynamiclib_unix.o

# build targets
all: $(TARGET)

clean:
	rm -f *.o *.so

# build rules
.c.o:
	$(CC) $(CFLAGS) $(SRC_FLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS)
	$(CC)  -shared $^ $(LDFLAGS) $(PLUGIN_LDFLAGS) $(SRC_LIBS) -o $@
#	$(STRIP) $@

