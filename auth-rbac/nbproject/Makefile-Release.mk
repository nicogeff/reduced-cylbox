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
	${OBJECTDIR}/src/auth_authorizations.o \
	${OBJECTDIR}/src/auth_global_rbac.o \
	${OBJECTDIR}/src/auth_mgt.o \
	${OBJECTDIR}/src/auth_roles.o \
	${OBJECTDIR}/src/auth_sessions.o \
	${OBJECTDIR}/src/auth_users.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1 \
	${TESTDIR}/TestFiles/f2

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/src/myAuthRbac_test_authent.o \
	${TESTDIR}/tests/src/myAuthRbac_test_mgt.o

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libauth-rbac.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libauth-rbac.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libauth-rbac.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libauth-rbac.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libauth-rbac.a

${OBJECTDIR}/src/auth_authorizations.o: src/auth_authorizations.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/auth_authorizations.o src/auth_authorizations.c

${OBJECTDIR}/src/auth_global_rbac.o: src/auth_global_rbac.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/auth_global_rbac.o src/auth_global_rbac.c

${OBJECTDIR}/src/auth_mgt.o: src/auth_mgt.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/auth_mgt.o src/auth_mgt.c

${OBJECTDIR}/src/auth_roles.o: src/auth_roles.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/auth_roles.o src/auth_roles.c

${OBJECTDIR}/src/auth_sessions.o: src/auth_sessions.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/auth_sessions.o src/auth_sessions.c

${OBJECTDIR}/src/auth_users.o: src/auth_users.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/auth_users.o src/auth_users.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:
	cd ../tools && ${MAKE}  -f Makefile CONF=Debug
	cd ../nal && ${MAKE}  -f Makefile CONF=Debug

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/src/myAuthRbac_test_authent.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   ../tools/dist/Debug/GNU-Linux/libtools.a ../nal/dist/Debug/GNU-Linux/libnal.a `pkg-config --libs cunit` -lpthread  -ldl  -lm  -lrt   -lcunit 

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/src/myAuthRbac_test_mgt.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS}   ../tools/dist/Debug/GNU-Linux/libtools.a ../nal/dist/Debug/GNU-Linux/libnal.a `pkg-config --libs cunit` -lpthread  -ldl  -lm  -lrt   -lcunit 


${TESTDIR}/tests/src/myAuthRbac_test_authent.o: tests/src/myAuthRbac_test_authent.c 
	${MKDIR} -p ${TESTDIR}/tests/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -Iinclude -I../../ng_common/nal/inc -lpthread -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/src/myAuthRbac_test_authent.o tests/src/myAuthRbac_test_authent.c


${TESTDIR}/tests/src/myAuthRbac_test_mgt.o: tests/src/myAuthRbac_test_mgt.c 
	${MKDIR} -p ${TESTDIR}/tests/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -Iinclude -I../../ng_common/nal/inc -lpthread -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/src/myAuthRbac_test_mgt.o tests/src/myAuthRbac_test_mgt.c


${OBJECTDIR}/src/auth_authorizations_nomain.o: ${OBJECTDIR}/src/auth_authorizations.o src/auth_authorizations.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/auth_authorizations.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/auth_authorizations_nomain.o src/auth_authorizations.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/auth_authorizations.o ${OBJECTDIR}/src/auth_authorizations_nomain.o;\
	fi

${OBJECTDIR}/src/auth_global_rbac_nomain.o: ${OBJECTDIR}/src/auth_global_rbac.o src/auth_global_rbac.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/auth_global_rbac.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/auth_global_rbac_nomain.o src/auth_global_rbac.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/auth_global_rbac.o ${OBJECTDIR}/src/auth_global_rbac_nomain.o;\
	fi

${OBJECTDIR}/src/auth_mgt_nomain.o: ${OBJECTDIR}/src/auth_mgt.o src/auth_mgt.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/auth_mgt.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/auth_mgt_nomain.o src/auth_mgt.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/auth_mgt.o ${OBJECTDIR}/src/auth_mgt_nomain.o;\
	fi

${OBJECTDIR}/src/auth_roles_nomain.o: ${OBJECTDIR}/src/auth_roles.o src/auth_roles.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/auth_roles.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/auth_roles_nomain.o src/auth_roles.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/auth_roles.o ${OBJECTDIR}/src/auth_roles_nomain.o;\
	fi

${OBJECTDIR}/src/auth_sessions_nomain.o: ${OBJECTDIR}/src/auth_sessions.o src/auth_sessions.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/auth_sessions.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/auth_sessions_nomain.o src/auth_sessions.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/auth_sessions.o ${OBJECTDIR}/src/auth_sessions_nomain.o;\
	fi

${OBJECTDIR}/src/auth_users_nomain.o: ${OBJECTDIR}/src/auth_users.o src/auth_users.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/auth_users.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Wall -Iinc -I../common/inc -I../crypto-al/plf/linux/inc -I../crypto-al/inc -I../entity-mgt/inc -I../nal/src/ip -I../nal/src/libc -I../nal/src/rtos -I../nal/inc -I../nal/plf/linux/inc -I../tools/inc -I../log_monitor/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/auth_users_nomain.o src/auth_users.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/auth_users.o ${OBJECTDIR}/src/auth_users_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	    ${TESTDIR}/TestFiles/f2 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
