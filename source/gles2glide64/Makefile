PIC ?= 1
NO_ASM := 1
CFLAGS += -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -mtune=cortex-a8 -fsigned-char -DNO_ASM -DPAULSCODE -Ofast -flto -fuse-linker-plugin
CFLAGS += -I ../../../../mupen64plus-bundle-src-2.0/source/mupen64plus-core/src/api/
CFLAGS += `sdl-config --cflags`

SHARED = -shared
SRCDIR := src

MODULE := gles2glide64
SHARED_LIBRARIES := SDL core
STATIC_LIBRARIES := png
ARM_MODE := arm

C_INCLUDES :=         \
    $(M64P_API_INCLUDES)    \
    $(SDL_INCLUDES)         \

SOURCE :=                  \
    $(SRCDIR)/Glitch64/combiner.cpp             \
    $(SRCDIR)/Glitch64/geometry.cpp             \
    $(SRCDIR)/Glitch64/glitchmain.cpp           \
    $(SRCDIR)/Glitch64/textures.cpp             \
    $(SRCDIR)/Glide64/osal_dynamiclib_unix.c    \
    $(SRCDIR)/Glide64/3dmath.cpp                \
    $(SRCDIR)/Glide64/Combine.cpp               \
    $(SRCDIR)/Glide64/Config.cpp                \
    $(SRCDIR)/Glide64/CRC.cpp                   \
    $(SRCDIR)/Glide64/Debugger.cpp              \
    $(SRCDIR)/Glide64/DepthBufferRender.cpp     \
    $(SRCDIR)/Glide64/FBtoScreen.cpp            \
    $(SRCDIR)/Glide64/FrameSkipper.cpp          \
    $(SRCDIR)/Glide64/Ini.cpp                   \
    $(SRCDIR)/Glide64/Keys.cpp                  \
    $(SRCDIR)/Glide64/Main.cpp                  \
    $(SRCDIR)/Glide64/rdp.cpp                   \
    $(SRCDIR)/Glide64/TexBuffer.cpp             \
    $(SRCDIR)/Glide64/TexCache.cpp              \
    $(SRCDIR)/Glide64/ticks.c                   \
    $(SRCDIR)/Glide64/Util.cpp                  \
	
CFLAGS +=         \
    $(COMMON_CFLAGS)    \
    -D__CRC_OPT         \
    -D__HASHMAP_OPT     \
    -D__TRIBUFFER_OPT   \
    -D__VEC4_OPT        \
    -DUSE_SDL           \
    -fsigned-char       \
    #-DSDL_NO_COMPAT     \
    
ifeq ($(PIC), 1)
  CFLAGS += -fPIC
else
  CFLAGS += -fno-PIC
endif

CPPFLAGS := $(CPPFLAGS)
    
LDLIBS :=         \
    -lGLESv2		\
	-lEGL			\
	-lrt			\

# Use for ARM7a:
CFLAGS += -DARM_ASM
CFLAGS += -D__NEON_OPT

LDFLAGS += $(SHARED)

include $(BUILD_SHARED_LIBRARY)

# set base program pointers and flags
CC        = $(CROSS_COMPILE)gcc
CXX       = $(CROSS_COMPILE)g++
RM       ?= rm -f
INSTALL  ?= install
MKDIR ?= mkdir -p
COMPILE.c = $(Q_CC)$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -gdwarf-2 -flto -fuse-linker-plugin -c
COMPILE.cc = $(Q_CXX)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -gdwarf-2 -flto -fuse-linker-plugin -c
LINK.o = $(Q_LD)$(CXX) $(CXXFLAGS) $(LDFLAGS) $(TARGET_ARCH) -gdwarf-2 -flto -fuse-linker-plugin

# set installation options
ifeq ($(PREFIX),)
  PREFIX := /usr/local
endif
ifeq ($(SHAREDIR),)
  SHAREDIR := $(PREFIX)/share/mupen64plus
endif
ifeq ($(LIBDIR),)
  LIBDIR := $(PREFIX)/lib
endif
ifeq ($(PLUGINDIR),)
  PLUGINDIR := $(LIBDIR)/mupen64plus
endif

OBJDIR = _obj$(POSTFIX)

# generate a list of object files build, make a temporary directory for them
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(filter %.c, $(SOURCE)))
OBJECTS += $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(filter %.cpp, $(SOURCE)))
OBJDIRS = $(dir $(OBJECTS))
$(shell $(MKDIR) $(OBJDIRS))

# build targets
TARGET = mupen64plus-video-gles2n64.so

targets:
	@echo "Mupen64plus-video-rice N64 Graphics plugin makefile. "
	@echo "  Targets:"
	@echo "    all           == Build Mupen64plus-video-rice plugin"
	@echo "    clean         == remove object files"
	@echo "    rebuild       == clean and re-build all"
	@echo "    install       == Install Mupen64Plus-video-rice plugin"
	@echo "    uninstall     == Uninstall Mupen64Plus-video-rice plugin"
	@echo "  Options:"
	@echo "    BITS=32       == build 32-bit binaries on 64-bit machine"
	@echo "    NO_ASM=1      == build without inline assembly code (x86 MMX/SSE)"
	@echo "    APIDIR=path   == path to find Mupen64Plus Core headers"
	@echo "    OPTFLAGS=flag == compiler optimization (default: -O3)"
	@echo "    WARNFLAGS=flag == compiler warning levels (default: -Wall)"
	@echo "    PIC=(1|0)     == Force enable/disable of position independent code"
	@echo "    POSTFIX=name  == String added to the name of the the build (default: '')"
	@echo "  Install Options:"
	@echo "    PREFIX=path   == install/uninstall prefix (default: /usr/local)"
	@echo "    SHAREDIR=path == path to install shared data files (default: PREFIX/share/mupen64plus)"
	@echo "    LIBDIR=path   == library prefix (default: PREFIX/lib)"
	@echo "    PLUGINDIR=path == path to install plugin libraries (default: LIBDIR/mupen64plus)"
	@echo "    DESTDIR=path  == path to prepend to all installation paths (only for packagers)"
	@echo "  Debugging Options:"
	@echo "    DEBUG=1       == add debugging symbols"
	@echo "    V=1           == show verbose compiler output"

all: $(TARGET)

install: $(TARGET)
	$(INSTALL) -d "$(DESTDIR)$(PLUGINDIR)"
	$(INSTALL) -m 0644 $(INSTALL_STRIP_FLAG) $(TARGET) "$(DESTDIR)$(PLUGINDIR)"
	$(INSTALL) -d "$(DESTDIR)$(SHAREDIR)"

uninstall:
	$(RM) "$(DESTDIR)$(PLUGINDIR)/$(TARGET)"

clean:
	$(RM) -r $(OBJDIR) $(TARGET)

rebuild: clean all

# build dependency files
CFLAGS += -MD
-include $(OBJECTS:.o=.d)

CXXFLAGS += $(CFLAGS)

# standard build rules
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(COMPILE.c) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(COMPILE.cc) -o $@ $<

$(TARGET): $(OBJECTS)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: all clean install uninstall targets
