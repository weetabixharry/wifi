#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/sdl_audio.o \
	${OBJECTDIR}/src/sdl_button.o \
	${OBJECTDIR}/src/sdl_gui.o \
	${OBJECTDIR}/src/sdl_texture.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../../../thirdparty/SDL2-2.0.9\ Dev/i686-w64-mingw32/lib -L../../../../thirdparty/SDL2_image-2.0.4\ Dev/i686-w64-mingw32/lib -L../../../../thirdparty/SDL2_ttf-2.0.14\ Dev/i686-w64-mingw32/lib -L../../../../thirdparty/SDL2_mixer-2.0.4\ Dev/i686-w64-mingw32/lib

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sdlsandpit.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sdlsandpit.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sdlsandpit ${OBJECTFILES} ${LDLIBSOPTIONS} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../thirdparty/SDL2-2.0.9\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_image-2.0.4\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_ttf-2.0.14\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_mixer-2.0.4\ Dev/i686-w64-mingw32/include/SDL2 -Isrc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/src/sdl_audio.o: src/sdl_audio.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../thirdparty/SDL2-2.0.9\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_image-2.0.4\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_ttf-2.0.14\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_mixer-2.0.4\ Dev/i686-w64-mingw32/include/SDL2 -Isrc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sdl_audio.o src/sdl_audio.cpp

${OBJECTDIR}/src/sdl_button.o: src/sdl_button.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../thirdparty/SDL2-2.0.9\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_image-2.0.4\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_ttf-2.0.14\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_mixer-2.0.4\ Dev/i686-w64-mingw32/include/SDL2 -Isrc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sdl_button.o src/sdl_button.cpp

${OBJECTDIR}/src/sdl_gui.o: src/sdl_gui.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../thirdparty/SDL2-2.0.9\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_image-2.0.4\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_ttf-2.0.14\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_mixer-2.0.4\ Dev/i686-w64-mingw32/include/SDL2 -Isrc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sdl_gui.o src/sdl_gui.cpp

${OBJECTDIR}/src/sdl_texture.o: src/sdl_texture.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../thirdparty/SDL2-2.0.9\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_image-2.0.4\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_ttf-2.0.14\ Dev/i686-w64-mingw32/include/SDL2 -I../../../../thirdparty/SDL2_mixer-2.0.4\ Dev/i686-w64-mingw32/include/SDL2 -Isrc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sdl_texture.o src/sdl_texture.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
