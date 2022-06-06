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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o


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
LDLIBSOPTIONS=../../../entity-mgt/dist/Debug/GNU-Linux/libentity-mgt.a ../../../web-server/dist/Debug/GNU-Linux/libweb-server.a ../../../auth-rbac/dist/Debug/GNU-Linux/libauth-rbac.a ../../../crypto-al/dist/Debug/GNU-Linux/libcrypto-al.a ../../../tools/dist/Debug/GNU-Linux/libtools.a ../../../nal/dist/Debug/GNU-Linux/libnal.a ../../../3-party/mbedtls/library/libmbedtls.a ../../../3-party/mbedtls/library/libmbedcrypto.a ../../../3-party/mbedtls/library/libmbedx509.a ../../../3-party/civetweb/libcivetweb.a -lrt -lm -lpthread `pkg-config --libs openssl` -ldl  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/devsec

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/devsec: ../../../entity-mgt/dist/Debug/GNU-Linux/libentity-mgt.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/devsec: ../../../web-server/dist/Debug/GNU-Linux/libweb-server.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/devsec: ../../../auth-rbac/dist/Debug/GNU-Linux/libauth-rbac.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/devsec: ../../../crypto-al/dist/Debug/GNU-Linux/libcrypto-al.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/devsec: ../../../tools/dist/Debug/GNU-Linux/libtools.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/devsec: ../../../nal/dist/Debug/GNU-Linux/libnal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/devsec: ../../../3-party/mbedtls/library/libmbedtls.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/devsec: ../../../3-party/mbedtls/library/libmbedcrypto.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/devsec: ../../../3-party/mbedtls/library/libmbedx509.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/devsec: ../../../3-party/civetweb/libcivetweb.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/devsec: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/devsec ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -I../../../common/inc -I../../../tools/inc -I../../../nal/inc -I../../../nal/plf/linux/inc `pkg-config --cflags openssl`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

# Subprojects
.build-subprojects:
	cd ../../../entity-mgt && ${MAKE}  -f Makefile CONF=Debug
	cd ../../../web-server && ${MAKE}  -f Makefile CONF=Debug
	cd ../../../auth-rbac && ${MAKE}  -f Makefile CONF=Debug
	cd ../../../crypto-al && ${MAKE}  -f Makefile CONF=Debug
	cd ../../../tools && ${MAKE}  -f Makefile CONF=Debug
	cd ../../../nal && ${MAKE}  -f Makefile CONF=Debug
	cd ../../../tools && ${MAKE}  -f Makefile CONF=Debug
	cd ../../../nal && ${MAKE}  -f Makefile CONF=Debug
	cd ../../../entity-mgt && ${MAKE}  -f Makefile CONF=Debug
	cd ../../../auth-rbac && ${MAKE}  -f Makefile CONF=Debug
	cd ../../../crypto-al && ${MAKE}  -f Makefile CONF=Debug
	cd ../../../web-server && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:
	cd ../../../entity-mgt && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../../web-server && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../../auth-rbac && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../../crypto-al && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../../tools && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../../nal && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../../tools && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../../nal && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../../entity-mgt && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../../auth-rbac && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../../crypto-al && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../../web-server && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
