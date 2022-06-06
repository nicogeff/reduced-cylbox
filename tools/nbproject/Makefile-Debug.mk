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
	${OBJECTDIR}/src/tools_chainedList.o \
	${OBJECTDIR}/src/tools_console.o \
	${OBJECTDIR}/src/tools_conversion.o \
	${OBJECTDIR}/src/tools_fileManip.o \
	${OBJECTDIR}/src/tools_parserJson.o \
	${OBJECTDIR}/src/tools_parserJsonTools.o \
	${OBJECTDIR}/src/tools_strManip.o \
	${OBJECTDIR}/src/tools_timeManip.o \
	${OBJECTDIR}/src/tools_trace.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/tools_chainedList_tests/tools_chainedList_main.o \
	${TESTDIR}/tests/tools_chainedList_tests/tools_chainedList_tests.o \
	${TESTDIR}/tests/tools_parserJson_tests/tools_parserJsonTools_tests.o \
	${TESTDIR}/tests/tools_parserJson_tests/tools_parserJson_main.o \
	${TESTDIR}/tests/tools_parserJson_tests/tools_parserJson_tests.o \
	${TESTDIR}/tests/tools_strManip_tests/tools_strManip_tests.o

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtools.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtools.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtools.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtools.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtools.a

${OBJECTDIR}/src/tools_chainedList.o: src/tools_chainedList.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_chainedList.o src/tools_chainedList.c

${OBJECTDIR}/src/tools_console.o: src/tools_console.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_console.o src/tools_console.c

${OBJECTDIR}/src/tools_conversion.o: src/tools_conversion.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_conversion.o src/tools_conversion.c

${OBJECTDIR}/src/tools_fileManip.o: src/tools_fileManip.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_fileManip.o src/tools_fileManip.c

${OBJECTDIR}/src/tools_parserJson.o: src/tools_parserJson.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_parserJson.o src/tools_parserJson.c

${OBJECTDIR}/src/tools_parserJsonTools.o: src/tools_parserJsonTools.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_parserJsonTools.o src/tools_parserJsonTools.c

${OBJECTDIR}/src/tools_strManip.o: src/tools_strManip.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_strManip.o src/tools_strManip.c

${OBJECTDIR}/src/tools_timeManip.o: src/tools_timeManip.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_timeManip.o src/tools_timeManip.c

${OBJECTDIR}/src/tools_trace.o: src/tools_trace.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_trace.o src/tools_trace.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:
	cd ../nal && ${MAKE}  -f Makefile CONF=Debug

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/tools_chainedList_tests/tools_chainedList_main.o ${TESTDIR}/tests/tools_chainedList_tests/tools_chainedList_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS}   ../nal/dist/Debug/GNU-Linux/libnal.a -lm -lcunit 

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/tools_parserJson_tests/tools_parserJsonTools_tests.o ${TESTDIR}/tests/tools_parserJson_tests/tools_parserJson_main.o ${TESTDIR}/tests/tools_parserJson_tests/tools_parserJson_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS}   ../nal/dist/Debug/GNU-Linux/libnal.a -lm -lcunit 

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/tools_strManip_tests/tools_strManip_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   ../nal/dist/Debug/GNU-Linux/libnal.a -lm -lcunit 


${TESTDIR}/tests/tools_chainedList_tests/tools_chainedList_main.o: tests/tools_chainedList_tests/tools_chainedList_main.c 
	${MKDIR} -p ${TESTDIR}/tests/tools_chainedList_tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Iinc -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/tools_chainedList_tests/tools_chainedList_main.o tests/tools_chainedList_tests/tools_chainedList_main.c


${TESTDIR}/tests/tools_chainedList_tests/tools_chainedList_tests.o: tests/tools_chainedList_tests/tools_chainedList_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/tools_chainedList_tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Iinc -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/tools_chainedList_tests/tools_chainedList_tests.o tests/tools_chainedList_tests/tools_chainedList_tests.c


${TESTDIR}/tests/tools_parserJson_tests/tools_parserJsonTools_tests.o: tests/tools_parserJson_tests/tools_parserJsonTools_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/tools_parserJson_tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Iinc -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/tools_parserJson_tests/tools_parserJsonTools_tests.o tests/tools_parserJson_tests/tools_parserJsonTools_tests.c


${TESTDIR}/tests/tools_parserJson_tests/tools_parserJson_main.o: tests/tools_parserJson_tests/tools_parserJson_main.c 
	${MKDIR} -p ${TESTDIR}/tests/tools_parserJson_tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Iinc -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/tools_parserJson_tests/tools_parserJson_main.o tests/tools_parserJson_tests/tools_parserJson_main.c


${TESTDIR}/tests/tools_parserJson_tests/tools_parserJson_tests.o: tests/tools_parserJson_tests/tools_parserJson_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/tools_parserJson_tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Iinc -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/tools_parserJson_tests/tools_parserJson_tests.o tests/tools_parserJson_tests/tools_parserJson_tests.c


${TESTDIR}/tests/tools_strManip_tests/tools_strManip_tests.o: tests/tools_strManip_tests/tools_strManip_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/tools_strManip_tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Iinc -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/tools_strManip_tests/tools_strManip_tests.o tests/tools_strManip_tests/tools_strManip_tests.c


${OBJECTDIR}/src/tools_chainedList_nomain.o: ${OBJECTDIR}/src/tools_chainedList.o src/tools_chainedList.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/tools_chainedList.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_chainedList_nomain.o src/tools_chainedList.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/tools_chainedList.o ${OBJECTDIR}/src/tools_chainedList_nomain.o;\
	fi

${OBJECTDIR}/src/tools_console_nomain.o: ${OBJECTDIR}/src/tools_console.o src/tools_console.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/tools_console.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_console_nomain.o src/tools_console.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/tools_console.o ${OBJECTDIR}/src/tools_console_nomain.o;\
	fi

${OBJECTDIR}/src/tools_conversion_nomain.o: ${OBJECTDIR}/src/tools_conversion.o src/tools_conversion.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/tools_conversion.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_conversion_nomain.o src/tools_conversion.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/tools_conversion.o ${OBJECTDIR}/src/tools_conversion_nomain.o;\
	fi

${OBJECTDIR}/src/tools_fileManip_nomain.o: ${OBJECTDIR}/src/tools_fileManip.o src/tools_fileManip.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/tools_fileManip.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_fileManip_nomain.o src/tools_fileManip.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/tools_fileManip.o ${OBJECTDIR}/src/tools_fileManip_nomain.o;\
	fi

${OBJECTDIR}/src/tools_parserJson_nomain.o: ${OBJECTDIR}/src/tools_parserJson.o src/tools_parserJson.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/tools_parserJson.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_parserJson_nomain.o src/tools_parserJson.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/tools_parserJson.o ${OBJECTDIR}/src/tools_parserJson_nomain.o;\
	fi

${OBJECTDIR}/src/tools_parserJsonTools_nomain.o: ${OBJECTDIR}/src/tools_parserJsonTools.o src/tools_parserJsonTools.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/tools_parserJsonTools.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_parserJsonTools_nomain.o src/tools_parserJsonTools.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/tools_parserJsonTools.o ${OBJECTDIR}/src/tools_parserJsonTools_nomain.o;\
	fi

${OBJECTDIR}/src/tools_strManip_nomain.o: ${OBJECTDIR}/src/tools_strManip.o src/tools_strManip.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/tools_strManip.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_strManip_nomain.o src/tools_strManip.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/tools_strManip.o ${OBJECTDIR}/src/tools_strManip_nomain.o;\
	fi

${OBJECTDIR}/src/tools_timeManip_nomain.o: ${OBJECTDIR}/src/tools_timeManip.o src/tools_timeManip.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/tools_timeManip.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_timeManip_nomain.o src/tools_timeManip.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/tools_timeManip.o ${OBJECTDIR}/src/tools_timeManip_nomain.o;\
	fi

${OBJECTDIR}/src/tools_trace_nomain.o: ${OBJECTDIR}/src/tools_trace.o src/tools_trace.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/tools_trace.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -Iinc -I../nal/inc -I../nal/src/stor -I../nal/plf/linux/inc -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tools_trace_nomain.o src/tools_trace.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/tools_trace.o ${OBJECTDIR}/src/tools_trace_nomain.o;\
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
