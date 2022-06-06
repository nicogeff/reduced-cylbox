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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/lib_main.o


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
LDLIBSOPTIONS=../../../entity-mgt/dist/Release/GNU-Linux/libentity-mgt.a ../../../web-server/dist/Release/GNU-Linux/libweb-server.a ../../../auth-rbac/dist/Release/GNU-Linux/libauth-rbac.a ../../../crypto-al/dist/Release/GNU-Linux/libcrypto-al.a ../../../tools/dist/Release/GNU-Linux/libtools.a ../../../nal/dist/Release/GNU-Linux/libnal.a ../../../3-party/mbedtls/library/libmbedtls.a ../../../3-party/mbedtls/library/libmbedcrypto.a ../../../3-party/mbedtls/library/libmbedx509.a ../../../3-party/civetweb/libcivetweb.a -lm -lpthread -ldl `pkg-config --libs libgpiod` `pkg-config --libs openssl`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdevsec.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdevsec.${CND_DLIB_EXT}: ../../../entity-mgt/dist/Release/GNU-Linux/libentity-mgt.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdevsec.${CND_DLIB_EXT}: ../../../web-server/dist/Release/GNU-Linux/libweb-server.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdevsec.${CND_DLIB_EXT}: ../../../auth-rbac/dist/Release/GNU-Linux/libauth-rbac.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdevsec.${CND_DLIB_EXT}: ../../../crypto-al/dist/Release/GNU-Linux/libcrypto-al.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdevsec.${CND_DLIB_EXT}: ../../../tools/dist/Release/GNU-Linux/libtools.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdevsec.${CND_DLIB_EXT}: ../../../nal/dist/Release/GNU-Linux/libnal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdevsec.${CND_DLIB_EXT}: ../../../3-party/mbedtls/library/libmbedtls.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdevsec.${CND_DLIB_EXT}: ../../../3-party/mbedtls/library/libmbedcrypto.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdevsec.${CND_DLIB_EXT}: ../../../3-party/mbedtls/library/libmbedx509.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdevsec.${CND_DLIB_EXT}: ../../../3-party/civetweb/libcivetweb.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdevsec.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdevsec.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/lib_main.o: lib_main.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Wall -I../../../common/inc -I../../../tools/inc -I../../../nal/inc -I../../../nal/plf/linux/inc `pkg-config --cflags libgpiod` `pkg-config --cflags openssl`   -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib_main.o lib_main.c

# Subprojects
.build-subprojects:
	cd ../../../entity-mgt && ${MAKE}  -f Makefile CONF=Release
	cd ../../../web-server && ${MAKE}  -f Makefile CONF=Release
	cd ../../../auth-rbac && ${MAKE}  -f Makefile CONF=Release
	cd ../../../crypto-al && ${MAKE}  -f Makefile CONF=Release
	cd ../../../tools && ${MAKE}  -f Makefile CONF=Release
	cd ../../../nal && ${MAKE}  -f Makefile CONF=Release
	cd ../../../nal && ${MAKE}  -f Makefile CONF=Release
	cd ../../../tools && ${MAKE}  -f Makefile CONF=Release
	cd ../../../entity-mgt && ${MAKE}  -f Makefile CONF=Release
	cd ../../../auth-rbac && ${MAKE}  -f Makefile CONF=Release
	cd ../../../web-server && ${MAKE}  -f Makefile CONF=Release
	cd ../../../crypto-al && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:
	cd ../../../entity-mgt && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../../web-server && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../../auth-rbac && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../../crypto-al && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../../tools && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../../nal && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../../nal && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../../tools && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../../entity-mgt && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../../auth-rbac && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../../web-server && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../../crypto-al && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
