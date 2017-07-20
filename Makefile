# (C)2004-2008 SourceMod Development Team
# Original Makefile written by David "BAILOPAN" Anderson

SRCDS_BASE ?= ../../l4d2_ds
HL2SDK_L4D2 ?= ../hl2sdk-l4d2
HL2SDK_L4D ?= ../hl2sdk-l4d
MMSOURCE18 ?= ../mmsource-1.10
L4D ?= 2

#####################################
### EDIT BELOW FOR OTHER PROJECTS ###
#####################################

PROJECT = tickrate_enabler

SH_OBJECTS = sourcehook.cpp sourcehook_impl_chookidman.cpp sourcehook_impl_cproto.cpp sourcehook_hookmangen.cpp \
	sourcehook_impl_chookmaninfo.cpp sourcehook_impl_cvfnptr.cpp

CP_OBJECTS = patchmanager.cpp

OBJECTS = maxrate_patches.cpp boomervomitpatch.cpp tickrate_enabler.cpp memutils.cpp \
	$(addprefix codepatch/,$(CP_OBJECTS)) $(addprefix sourcehook/,$(SH_OBJECTS))

##############################################
### CONFIGURE ANY OTHER FLAGS/OPTIONS HERE ###
##############################################

C_OPT_FLAGS = -DNDEBUG -O3 -funroll-loops -pipe -fno-strict-aliasing
C_DEBUG_FLAGS = -D_DEBUG -DDEBUG -g -ggdb3
C_GCC4_FLAGS = -fvisibility=hidden
CPP_GCC4_FLAGS = -fvisibility-inlines-hidden
CPP = gcc

ifeq "$(L4D)" "1"
	HL2PUB = $(HL2SDK_L4D)/public
	HL2LIB = $(HL2SDK_L4D)/lib/linux
	SRCDS = $(SRCDS_BASE)/l4d
	CFLAGS += -DL4D1
	LIBPOSTFIX = 
else
	HL2PUB = $(HL2SDK_L4D2)/public
	HL2LIB = $(HL2SDK_L4D2)/lib/linux
	SRCDS = $(SRCDS_BASE)/
	CFLAGS += -DL4D2
	LIBPOSTFIX = _srv
endif

LINK += $(HL2LIB)/tier1_i486.a $(HL2LIB)/mathlib_i486.a libvstdlib$(LIBPOSTFIX).so libtier0$(LIBPOSTFIX).so

INCLUDE += -I. -I$(HL2PUB) -I$(HL2PUB)/tier0 -I$(HL2PUB)/tier1

LINK += -m32 -ldl -lm

CFLAGS += -D_LINUX -Dstricmp=strcasecmp -D_stricmp=strcasecmp -D_strnicmp=strncasecmp -Dstrnicmp=strncasecmp \
        -D_snprintf=snprintf -D_vsnprintf=vsnprintf -D_alloca=alloca -Dstrcmpi=strcasecmp -Wall -Werror -Wno-switch \
        -Wno-error=uninitialized -Wno-unused -Wno-error=delete-non-virtual-dtor -mfpmath=sse -msse -DSOURCEMOD_BUILD -DHAVE_STDINT_H -m32 

CPPFLAGS += -Wno-non-virtual-dtor -fno-rtti -fno-threadsafe-statics -Wno-overloaded-virtual -Wno-uninitialized -Wno-delete-non-virtual-dtor

################################################
### DO NOT EDIT BELOW HERE FOR MOST PROJECTS ###
################################################

ifeq "$(DEBUG)" "true"
	ifeq "$(L4D)" "1"
		BIN_DIR = Debug.left4dead
	else
		BIN_DIR = Debug.left4dead2
	endif
	CFLAGS += $(C_DEBUG_FLAGS)
else
	ifeq "$(L4D)" "1"
		BIN_DIR = Release.left4dead
	else
		BIN_DIR = Release.left4dead2
	endif
	CFLAGS += $(C_OPT_FLAGS)
endif

OS := $(shell uname -s)
ifeq "$(OS)" "Darwin"
	LINK += -dynamiclib
	BINARY = $(PROJECT).dylib
else
	LINK += -static-libgcc -shared
	BINARY = $(PROJECT).so
endif

GCC_VERSION := $(shell $(CPP) -dumpversion >&1 | cut -b1)
ifeq "$(GCC_VERSION)" "4"
	CFLAGS += $(C_GCC4_FLAGS)
	CPPFLAGS += $(CPP_GCC4_FLAGS)
endif

OBJ_LINUX := $(OBJECTS:%.cpp=$(BIN_DIR)/%.o)

$(BIN_DIR)/%.o: %.cpp
	$(CPP) $(INCLUDE) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

all:
	ln -s $(SRCDS)/bin/libvstdlib$(LIBPOSTFIX).so libvstdlib$(LIBPOSTFIX).so;
	ln -s $(SRCDS)/bin/libtier0$(LIBPOSTFIX).so libtier0$(LIBPOSTFIX).so;
	mkdir -p $(BIN_DIR) $(BIN_DIR)/sourcehook $(BIN_DIR)/codepatch
	$(MAKE) -f Makefile extension

l4d1:
	$(MAKE) -f Makefile all L4D=1


extension: $(OBJ_LINUX)
	$(CPP) $(INCLUDE) $(OBJ_LINUX) $(LINK) -o $(BIN_DIR)/$(BINARY)

l4d1-debug:
	$(MAKE) -f Makefile all DEBUG=true L4D=1

debug:
	$(MAKE) -f Makefile all DEBUG=true

default: all

#clean:
#	find $(BIN_DIR) -iname *.o | xargs rm -f
#	rm -rf $(BIN_DIR)/$(BINARY)
#	rm ./*.so
clean:
	rm -rf Release.left4dead Release.left4dead2 Debug.left4dead Debug.left4dead2 libtier0_srv.so libvstdlib_srv.so

