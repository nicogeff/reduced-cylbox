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
	${OBJECTDIR}/src/entmgt_configure.o \
	${OBJECTDIR}/src/entmgt_initialize.o \
	${OBJECTDIR}/src/entmgt_launcher.o \
	${OBJECTDIR}/src/entmgt_start.o \
	${OBJECTDIR}/src/entmgt_stop.o \
	${OBJECTDIR}/src/entmgt_utils.o


# C Compiler Flags
CFLAGS=-fPIC

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libentity-mgt.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libentity-mgt.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libentity-mgt.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libentity-mgt.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libentity-mgt.a

${OBJECTDIR}/src/entmgt_configure.o: src/entmgt_configure.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -Iresource -I../common/inc -I../tools/inc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -I../crypto-al/inc -I../nal/src/net -I../nal/src/ip -I../crypto-al/inc -I../crypto-al/plf/linux/inc -I../nal/src/rtos -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/entmgt_configure.o src/entmgt_configure.c

${OBJECTDIR}/src/entmgt_initialize.o: src/entmgt_initialize.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -Iresource -I../common/inc -I../tools/inc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -I../crypto-al/inc -I../nal/src/net -I../nal/src/ip -I../crypto-al/inc -I../crypto-al/plf/linux/inc -I../nal/src/rtos -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/entmgt_initialize.o src/entmgt_initialize.c

${OBJECTDIR}/src/entmgt_launcher.o: src/entmgt_launcher.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -Iresource -I../common/inc -I../tools/inc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -I../crypto-al/inc -I../nal/src/net -I../nal/src/ip -I../crypto-al/inc -I../crypto-al/plf/linux/inc -I../nal/src/rtos -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/entmgt_launcher.o src/entmgt_launcher.c

${OBJECTDIR}/src/entmgt_start.o: src/entmgt_start.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -Iresource -I../common/inc -I../tools/inc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -I../crypto-al/inc -I../nal/src/net -I../nal/src/ip -I../crypto-al/inc -I../crypto-al/plf/linux/inc -I../nal/src/rtos -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/entmgt_start.o src/entmgt_start.c

${OBJECTDIR}/src/entmgt_stop.o: src/entmgt_stop.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -Iresource -I../common/inc -I../tools/inc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -I../crypto-al/inc -I../nal/src/net -I../nal/src/ip -I../crypto-al/inc -I../crypto-al/plf/linux/inc -I../nal/src/rtos -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/entmgt_stop.o src/entmgt_stop.c

${OBJECTDIR}/src/entmgt_utils.o: src/entmgt_utils.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -Iresource -I../common/inc -I../tools/inc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -I../crypto-al/inc -I../nal/src/net -I../nal/src/ip -I../crypto-al/inc -I../crypto-al/plf/linux/inc -I../nal/src/rtos -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/entmgt_utils.o src/entmgt_utils.c

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
