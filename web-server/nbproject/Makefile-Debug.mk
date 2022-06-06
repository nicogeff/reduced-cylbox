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
	${OBJECTDIR}/src/webserver_mcast.o \
	${OBJECTDIR}/src/webserver_mgt.o \
	${OBJECTDIR}/src/webserver_operations.o \
	${OBJECTDIR}/src/webserver_restful.o \
	${OBJECTDIR}/src/webserver_utils.o \
	${OBJECTDIR}/src/webserver_www.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libweb-server.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libweb-server.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libweb-server.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libweb-server.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libweb-server.a

${OBJECTDIR}/src/webserver_mcast.o: src/webserver_mcast.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Iinc -I../common/inc -I../nal/inc -I../nal/src/rtos -I../nal/plf/linux/inc -I../tools/inc -I../crypto-al/inc -I../crypto-al/plf/linux/inc -I../nal/src/net -I../nal/src/ip -I../entity-mgt/inc -I../3-party/civetweb/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/webserver_mcast.o src/webserver_mcast.c

${OBJECTDIR}/src/webserver_mgt.o: src/webserver_mgt.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Iinc -I../common/inc -I../nal/inc -I../nal/src/rtos -I../nal/plf/linux/inc -I../tools/inc -I../crypto-al/inc -I../crypto-al/plf/linux/inc -I../nal/src/net -I../nal/src/ip -I../entity-mgt/inc -I../3-party/civetweb/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/webserver_mgt.o src/webserver_mgt.c

${OBJECTDIR}/src/webserver_operations.o: src/webserver_operations.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Iinc -I../common/inc -I../nal/inc -I../nal/src/rtos -I../nal/plf/linux/inc -I../tools/inc -I../crypto-al/inc -I../crypto-al/plf/linux/inc -I../nal/src/net -I../nal/src/ip -I../entity-mgt/inc -I../3-party/civetweb/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/webserver_operations.o src/webserver_operations.c

${OBJECTDIR}/src/webserver_restful.o: src/webserver_restful.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Iinc -I../common/inc -I../nal/inc -I../nal/src/rtos -I../nal/plf/linux/inc -I../tools/inc -I../crypto-al/inc -I../crypto-al/plf/linux/inc -I../nal/src/net -I../nal/src/ip -I../entity-mgt/inc -I../3-party/civetweb/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/webserver_restful.o src/webserver_restful.c

${OBJECTDIR}/src/webserver_utils.o: src/webserver_utils.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Iinc -I../common/inc -I../nal/inc -I../nal/src/rtos -I../nal/plf/linux/inc -I../tools/inc -I../crypto-al/inc -I../crypto-al/plf/linux/inc -I../nal/src/net -I../nal/src/ip -I../entity-mgt/inc -I../3-party/civetweb/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/webserver_utils.o src/webserver_utils.c

${OBJECTDIR}/src/webserver_www.o: src/webserver_www.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Iinc -I../common/inc -I../nal/inc -I../nal/src/rtos -I../nal/plf/linux/inc -I../tools/inc -I../crypto-al/inc -I../crypto-al/plf/linux/inc -I../nal/src/net -I../nal/src/ip -I../entity-mgt/inc -I../3-party/civetweb/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/webserver_www.o src/webserver_www.c

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
