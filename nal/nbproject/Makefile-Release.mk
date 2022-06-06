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
	${OBJECTDIR}/src/ip/nal_ip_bsd_4_3.o \
	${OBJECTDIR}/src/libc/nal_libc_posix.o \
	${OBJECTDIR}/src/mem/nal_mem_alloc.o \
	${OBJECTDIR}/src/net/nal_net_linux.o \
	${OBJECTDIR}/src/rtos/nal_rtos_posix.o \
	${OBJECTDIR}/src/stor/nal_stor_stdio.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/nal_libc_tests/nal_libc_main.o \
	${TESTDIR}/tests/nal_libc_tests/nal_libc_tests.o \
	${TESTDIR}/tests/nal_net_tests/nal_net_main.o \
	${TESTDIR}/tests/nal_net_tests/nal_net_tests.o \
	${TESTDIR}/tests/nal_rtos_tests/nal_rtos_main.o \
	${TESTDIR}/tests/nal_rtos_tests/nal_rtos_mutexes_tests.o \
	${TESTDIR}/tests/nal_rtos_tests/nal_rtos_semaphores_tests.o

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnal.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnal.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnal.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnal.a

${OBJECTDIR}/src/ip/nal_ip_bsd_4_3.o: src/ip/nal_ip_bsd_4_3.c
	${MKDIR} -p ${OBJECTDIR}/src/ip
	${RM} "$@.d"
	$(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ip/nal_ip_bsd_4_3.o src/ip/nal_ip_bsd_4_3.c

${OBJECTDIR}/src/libc/nal_libc_posix.o: src/libc/nal_libc_posix.c
	${MKDIR} -p ${OBJECTDIR}/src/libc
	${RM} "$@.d"
	$(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/libc/nal_libc_posix.o src/libc/nal_libc_posix.c

${OBJECTDIR}/src/mem/nal_mem_alloc.o: src/mem/nal_mem_alloc.c
	${MKDIR} -p ${OBJECTDIR}/src/mem
	${RM} "$@.d"
	$(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mem/nal_mem_alloc.o src/mem/nal_mem_alloc.c

${OBJECTDIR}/src/net/nal_net_linux.o: src/net/nal_net_linux.c
	${MKDIR} -p ${OBJECTDIR}/src/net
	${RM} "$@.d"
	$(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/nal_net_linux.o src/net/nal_net_linux.c

${OBJECTDIR}/src/rtos/nal_rtos_posix.o: src/rtos/nal_rtos_posix.c
	${MKDIR} -p ${OBJECTDIR}/src/rtos
	${RM} "$@.d"
	$(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rtos/nal_rtos_posix.o src/rtos/nal_rtos_posix.c

${OBJECTDIR}/src/stor/nal_stor_stdio.o: src/stor/nal_stor_stdio.c
	${MKDIR} -p ${OBJECTDIR}/src/stor
	${RM} "$@.d"
	$(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/stor/nal_stor_stdio.o src/stor/nal_stor_stdio.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/nal_libc_tests/nal_libc_main.o ${TESTDIR}/tests/nal_libc_tests/nal_libc_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS}   

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/nal_net_tests/nal_net_main.o ${TESTDIR}/tests/nal_net_tests/nal_net_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS}   

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/nal_rtos_tests/nal_rtos_main.o ${TESTDIR}/tests/nal_rtos_tests/nal_rtos_mutexes_tests.o ${TESTDIR}/tests/nal_rtos_tests/nal_rtos_semaphores_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   


${TESTDIR}/tests/nal_libc_tests/nal_libc_main.o: tests/nal_libc_tests/nal_libc_main.c 
	${MKDIR} -p ${TESTDIR}/tests/nal_libc_tests
	${RM} "$@.d"
	$(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/nal_libc_tests/nal_libc_main.o tests/nal_libc_tests/nal_libc_main.c


${TESTDIR}/tests/nal_libc_tests/nal_libc_tests.o: tests/nal_libc_tests/nal_libc_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/nal_libc_tests
	${RM} "$@.d"
	$(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/nal_libc_tests/nal_libc_tests.o tests/nal_libc_tests/nal_libc_tests.c


${TESTDIR}/tests/nal_net_tests/nal_net_main.o: tests/nal_net_tests/nal_net_main.c 
	${MKDIR} -p ${TESTDIR}/tests/nal_net_tests
	${RM} "$@.d"
	$(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/nal_net_tests/nal_net_main.o tests/nal_net_tests/nal_net_main.c


${TESTDIR}/tests/nal_net_tests/nal_net_tests.o: tests/nal_net_tests/nal_net_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/nal_net_tests
	${RM} "$@.d"
	$(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/nal_net_tests/nal_net_tests.o tests/nal_net_tests/nal_net_tests.c


${TESTDIR}/tests/nal_rtos_tests/nal_rtos_main.o: tests/nal_rtos_tests/nal_rtos_main.c 
	${MKDIR} -p ${TESTDIR}/tests/nal_rtos_tests
	${RM} "$@.d"
	$(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/nal_rtos_tests/nal_rtos_main.o tests/nal_rtos_tests/nal_rtos_main.c


${TESTDIR}/tests/nal_rtos_tests/nal_rtos_mutexes_tests.o: tests/nal_rtos_tests/nal_rtos_mutexes_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/nal_rtos_tests
	${RM} "$@.d"
	$(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/nal_rtos_tests/nal_rtos_mutexes_tests.o tests/nal_rtos_tests/nal_rtos_mutexes_tests.c


${TESTDIR}/tests/nal_rtos_tests/nal_rtos_semaphores_tests.o: tests/nal_rtos_tests/nal_rtos_semaphores_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/nal_rtos_tests
	${RM} "$@.d"
	$(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/nal_rtos_tests/nal_rtos_semaphores_tests.o tests/nal_rtos_tests/nal_rtos_semaphores_tests.c


${OBJECTDIR}/src/ip/nal_ip_bsd_4_3_nomain.o: ${OBJECTDIR}/src/ip/nal_ip_bsd_4_3.o src/ip/nal_ip_bsd_4_3.c 
	${MKDIR} -p ${OBJECTDIR}/src/ip
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/ip/nal_ip_bsd_4_3.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ip/nal_ip_bsd_4_3_nomain.o src/ip/nal_ip_bsd_4_3.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/ip/nal_ip_bsd_4_3.o ${OBJECTDIR}/src/ip/nal_ip_bsd_4_3_nomain.o;\
	fi

${OBJECTDIR}/src/libc/nal_libc_posix_nomain.o: ${OBJECTDIR}/src/libc/nal_libc_posix.o src/libc/nal_libc_posix.c 
	${MKDIR} -p ${OBJECTDIR}/src/libc
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/libc/nal_libc_posix.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/libc/nal_libc_posix_nomain.o src/libc/nal_libc_posix.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/libc/nal_libc_posix.o ${OBJECTDIR}/src/libc/nal_libc_posix_nomain.o;\
	fi

${OBJECTDIR}/src/mem/nal_mem_alloc_nomain.o: ${OBJECTDIR}/src/mem/nal_mem_alloc.o src/mem/nal_mem_alloc.c 
	${MKDIR} -p ${OBJECTDIR}/src/mem
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/mem/nal_mem_alloc.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mem/nal_mem_alloc_nomain.o src/mem/nal_mem_alloc.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/mem/nal_mem_alloc.o ${OBJECTDIR}/src/mem/nal_mem_alloc_nomain.o;\
	fi

${OBJECTDIR}/src/net/nal_net_linux_nomain.o: ${OBJECTDIR}/src/net/nal_net_linux.o src/net/nal_net_linux.c 
	${MKDIR} -p ${OBJECTDIR}/src/net
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/net/nal_net_linux.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/nal_net_linux_nomain.o src/net/nal_net_linux.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/net/nal_net_linux.o ${OBJECTDIR}/src/net/nal_net_linux_nomain.o;\
	fi

${OBJECTDIR}/src/rtos/nal_rtos_posix_nomain.o: ${OBJECTDIR}/src/rtos/nal_rtos_posix.o src/rtos/nal_rtos_posix.c 
	${MKDIR} -p ${OBJECTDIR}/src/rtos
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/rtos/nal_rtos_posix.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rtos/nal_rtos_posix_nomain.o src/rtos/nal_rtos_posix.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/rtos/nal_rtos_posix.o ${OBJECTDIR}/src/rtos/nal_rtos_posix_nomain.o;\
	fi

${OBJECTDIR}/src/stor/nal_stor_stdio_nomain.o: ${OBJECTDIR}/src/stor/nal_stor_stdio.o src/stor/nal_stor_stdio.c 
	${MKDIR} -p ${OBJECTDIR}/src/stor
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/stor/nal_stor_stdio.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -Wall -Iinc -Isrc/ip -Isrc/libc -Isrc/mem -Isrc/net -Isrc/rtos -Isrc/stor -Iplf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/stor/nal_stor_stdio_nomain.o src/stor/nal_stor_stdio.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/stor/nal_stor_stdio.o ${OBJECTDIR}/src/stor/nal_stor_stdio_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
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
