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
	${OBJECTDIR}/src/cryptoal_base64_mbedtls.o \
	${OBJECTDIR}/src/cryptoal_base64_openssl.o \
	${OBJECTDIR}/src/cryptoal_certificate_mbedtls.o \
	${OBJECTDIR}/src/cryptoal_hash_mbedtls.o \
	${OBJECTDIR}/src/cryptoal_hash_openssl.o \
	${OBJECTDIR}/src/cryptoal_mgt.o \
	${OBJECTDIR}/src/cryptoal_rand_mbedtls.o \
	${OBJECTDIR}/src/cryptoal_rand_openssl.o \
	${OBJECTDIR}/src/cryptoal_signature_mbedtls.o \
	${OBJECTDIR}/src/cryptoal_signature_openssl.o \
	${OBJECTDIR}/src/cryptoal_tls_mbedtls.o \
	${OBJECTDIR}/src/cryptoal_tls_openssl.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1 \
	${TESTDIR}/TestFiles/f5 \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f4 \
	${TESTDIR}/TestFiles/f6

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/cryptoal_base64_tests/cryptoal_base64_main.o \
	${TESTDIR}/tests/cryptoal_base64_tests/cryptoal_base64_tests.o \
	${TESTDIR}/tests/cryptoal_certificate_tests/cryptoal_certificate_main.o \
	${TESTDIR}/tests/cryptoal_certificate_tests/cryptoal_certificate_tests.o \
	${TESTDIR}/tests/cryptoal_hash_tests/cryptoal_hash_main.o \
	${TESTDIR}/tests/cryptoal_hash_tests/cryptoal_hash_tests.o \
	${TESTDIR}/tests/cryptoal_rand_tests/cryptoal_rand_main.o \
	${TESTDIR}/tests/cryptoal_rand_tests/cryptoal_rand_tests.o \
	${TESTDIR}/tests/cryptoal_signature_tests/cryptoal_signature_main.o \
	${TESTDIR}/tests/cryptoal_signature_tests/cryptoal_signature_tests.o \
	${TESTDIR}/tests/cryptoal_tls_tests/cryptoal_tls_main.o \
	${TESTDIR}/tests/cryptoal_tls_tests/cryptoal_tls_tests.o

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcrypto-al.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcrypto-al.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcrypto-al.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcrypto-al.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcrypto-al.a

${OBJECTDIR}/src/cryptoal_base64_mbedtls.o: src/cryptoal_base64_mbedtls.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_base64_mbedtls.o src/cryptoal_base64_mbedtls.c

${OBJECTDIR}/src/cryptoal_base64_openssl.o: src/cryptoal_base64_openssl.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_base64_openssl.o src/cryptoal_base64_openssl.c

${OBJECTDIR}/src/cryptoal_certificate_mbedtls.o: src/cryptoal_certificate_mbedtls.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_certificate_mbedtls.o src/cryptoal_certificate_mbedtls.c

${OBJECTDIR}/src/cryptoal_hash_mbedtls.o: src/cryptoal_hash_mbedtls.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_hash_mbedtls.o src/cryptoal_hash_mbedtls.c

${OBJECTDIR}/src/cryptoal_hash_openssl.o: src/cryptoal_hash_openssl.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_hash_openssl.o src/cryptoal_hash_openssl.c

${OBJECTDIR}/src/cryptoal_mgt.o: src/cryptoal_mgt.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_mgt.o src/cryptoal_mgt.c

${OBJECTDIR}/src/cryptoal_rand_mbedtls.o: src/cryptoal_rand_mbedtls.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_rand_mbedtls.o src/cryptoal_rand_mbedtls.c

${OBJECTDIR}/src/cryptoal_rand_openssl.o: src/cryptoal_rand_openssl.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_rand_openssl.o src/cryptoal_rand_openssl.c

${OBJECTDIR}/src/cryptoal_signature_mbedtls.o: src/cryptoal_signature_mbedtls.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_signature_mbedtls.o src/cryptoal_signature_mbedtls.c

${OBJECTDIR}/src/cryptoal_signature_openssl.o: src/cryptoal_signature_openssl.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_signature_openssl.o src/cryptoal_signature_openssl.c

${OBJECTDIR}/src/cryptoal_tls_mbedtls.o: src/cryptoal_tls_mbedtls.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_tls_mbedtls.o src/cryptoal_tls_mbedtls.c

${OBJECTDIR}/src/cryptoal_tls_openssl.o: src/cryptoal_tls_openssl.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_tls_openssl.o src/cryptoal_tls_openssl.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/cryptoal_base64_tests/cryptoal_base64_main.o ${TESTDIR}/tests/cryptoal_base64_tests/cryptoal_base64_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   

${TESTDIR}/TestFiles/f5: ${TESTDIR}/tests/cryptoal_certificate_tests/cryptoal_certificate_main.o ${TESTDIR}/tests/cryptoal_certificate_tests/cryptoal_certificate_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f5 $^ ${LDLIBSOPTIONS}   -lcunit 

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/cryptoal_hash_tests/cryptoal_hash_main.o ${TESTDIR}/tests/cryptoal_hash_tests/cryptoal_hash_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS}   

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/cryptoal_rand_tests/cryptoal_rand_main.o ${TESTDIR}/tests/cryptoal_rand_tests/cryptoal_rand_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS}   

${TESTDIR}/TestFiles/f4: ${TESTDIR}/tests/cryptoal_signature_tests/cryptoal_signature_main.o ${TESTDIR}/tests/cryptoal_signature_tests/cryptoal_signature_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS}   -lcunit 

${TESTDIR}/TestFiles/f6: ${TESTDIR}/tests/cryptoal_tls_tests/cryptoal_tls_main.o ${TESTDIR}/tests/cryptoal_tls_tests/cryptoal_tls_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f6 $^ ${LDLIBSOPTIONS}   -lcunit 


${TESTDIR}/tests/cryptoal_base64_tests/cryptoal_base64_main.o: tests/cryptoal_base64_tests/cryptoal_base64_main.c 
	${MKDIR} -p ${TESTDIR}/tests/cryptoal_base64_tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/cryptoal_base64_tests/cryptoal_base64_main.o tests/cryptoal_base64_tests/cryptoal_base64_main.c


${TESTDIR}/tests/cryptoal_base64_tests/cryptoal_base64_tests.o: tests/cryptoal_base64_tests/cryptoal_base64_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/cryptoal_base64_tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/cryptoal_base64_tests/cryptoal_base64_tests.o tests/cryptoal_base64_tests/cryptoal_base64_tests.c


${TESTDIR}/tests/cryptoal_certificate_tests/cryptoal_certificate_main.o: tests/cryptoal_certificate_tests/cryptoal_certificate_main.c 
	${MKDIR} -p ${TESTDIR}/tests/cryptoal_certificate_tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/cryptoal_certificate_tests/cryptoal_certificate_main.o tests/cryptoal_certificate_tests/cryptoal_certificate_main.c


${TESTDIR}/tests/cryptoal_certificate_tests/cryptoal_certificate_tests.o: tests/cryptoal_certificate_tests/cryptoal_certificate_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/cryptoal_certificate_tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/cryptoal_certificate_tests/cryptoal_certificate_tests.o tests/cryptoal_certificate_tests/cryptoal_certificate_tests.c


${TESTDIR}/tests/cryptoal_hash_tests/cryptoal_hash_main.o: tests/cryptoal_hash_tests/cryptoal_hash_main.c 
	${MKDIR} -p ${TESTDIR}/tests/cryptoal_hash_tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/cryptoal_hash_tests/cryptoal_hash_main.o tests/cryptoal_hash_tests/cryptoal_hash_main.c


${TESTDIR}/tests/cryptoal_hash_tests/cryptoal_hash_tests.o: tests/cryptoal_hash_tests/cryptoal_hash_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/cryptoal_hash_tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/cryptoal_hash_tests/cryptoal_hash_tests.o tests/cryptoal_hash_tests/cryptoal_hash_tests.c


${TESTDIR}/tests/cryptoal_rand_tests/cryptoal_rand_main.o: tests/cryptoal_rand_tests/cryptoal_rand_main.c 
	${MKDIR} -p ${TESTDIR}/tests/cryptoal_rand_tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/cryptoal_rand_tests/cryptoal_rand_main.o tests/cryptoal_rand_tests/cryptoal_rand_main.c


${TESTDIR}/tests/cryptoal_rand_tests/cryptoal_rand_tests.o: tests/cryptoal_rand_tests/cryptoal_rand_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/cryptoal_rand_tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/cryptoal_rand_tests/cryptoal_rand_tests.o tests/cryptoal_rand_tests/cryptoal_rand_tests.c


${TESTDIR}/tests/cryptoal_signature_tests/cryptoal_signature_main.o: tests/cryptoal_signature_tests/cryptoal_signature_main.c 
	${MKDIR} -p ${TESTDIR}/tests/cryptoal_signature_tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/cryptoal_signature_tests/cryptoal_signature_main.o tests/cryptoal_signature_tests/cryptoal_signature_main.c


${TESTDIR}/tests/cryptoal_signature_tests/cryptoal_signature_tests.o: tests/cryptoal_signature_tests/cryptoal_signature_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/cryptoal_signature_tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/cryptoal_signature_tests/cryptoal_signature_tests.o tests/cryptoal_signature_tests/cryptoal_signature_tests.c


${TESTDIR}/tests/cryptoal_tls_tests/cryptoal_tls_main.o: tests/cryptoal_tls_tests/cryptoal_tls_main.c 
	${MKDIR} -p ${TESTDIR}/tests/cryptoal_tls_tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/cryptoal_tls_tests/cryptoal_tls_main.o tests/cryptoal_tls_tests/cryptoal_tls_main.c


${TESTDIR}/tests/cryptoal_tls_tests/cryptoal_tls_tests.o: tests/cryptoal_tls_tests/cryptoal_tls_tests.c 
	${MKDIR} -p ${TESTDIR}/tests/cryptoal_tls_tests
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/cryptoal_tls_tests/cryptoal_tls_tests.o tests/cryptoal_tls_tests/cryptoal_tls_tests.c


${OBJECTDIR}/src/cryptoal_base64_mbedtls_nomain.o: ${OBJECTDIR}/src/cryptoal_base64_mbedtls.o src/cryptoal_base64_mbedtls.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/cryptoal_base64_mbedtls.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_base64_mbedtls_nomain.o src/cryptoal_base64_mbedtls.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/cryptoal_base64_mbedtls.o ${OBJECTDIR}/src/cryptoal_base64_mbedtls_nomain.o;\
	fi

${OBJECTDIR}/src/cryptoal_base64_openssl_nomain.o: ${OBJECTDIR}/src/cryptoal_base64_openssl.o src/cryptoal_base64_openssl.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/cryptoal_base64_openssl.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_base64_openssl_nomain.o src/cryptoal_base64_openssl.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/cryptoal_base64_openssl.o ${OBJECTDIR}/src/cryptoal_base64_openssl_nomain.o;\
	fi

${OBJECTDIR}/src/cryptoal_certificate_mbedtls_nomain.o: ${OBJECTDIR}/src/cryptoal_certificate_mbedtls.o src/cryptoal_certificate_mbedtls.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/cryptoal_certificate_mbedtls.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_certificate_mbedtls_nomain.o src/cryptoal_certificate_mbedtls.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/cryptoal_certificate_mbedtls.o ${OBJECTDIR}/src/cryptoal_certificate_mbedtls_nomain.o;\
	fi

${OBJECTDIR}/src/cryptoal_hash_mbedtls_nomain.o: ${OBJECTDIR}/src/cryptoal_hash_mbedtls.o src/cryptoal_hash_mbedtls.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/cryptoal_hash_mbedtls.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_hash_mbedtls_nomain.o src/cryptoal_hash_mbedtls.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/cryptoal_hash_mbedtls.o ${OBJECTDIR}/src/cryptoal_hash_mbedtls_nomain.o;\
	fi

${OBJECTDIR}/src/cryptoal_hash_openssl_nomain.o: ${OBJECTDIR}/src/cryptoal_hash_openssl.o src/cryptoal_hash_openssl.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/cryptoal_hash_openssl.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_hash_openssl_nomain.o src/cryptoal_hash_openssl.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/cryptoal_hash_openssl.o ${OBJECTDIR}/src/cryptoal_hash_openssl_nomain.o;\
	fi

${OBJECTDIR}/src/cryptoal_mgt_nomain.o: ${OBJECTDIR}/src/cryptoal_mgt.o src/cryptoal_mgt.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/cryptoal_mgt.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_mgt_nomain.o src/cryptoal_mgt.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/cryptoal_mgt.o ${OBJECTDIR}/src/cryptoal_mgt_nomain.o;\
	fi

${OBJECTDIR}/src/cryptoal_rand_mbedtls_nomain.o: ${OBJECTDIR}/src/cryptoal_rand_mbedtls.o src/cryptoal_rand_mbedtls.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/cryptoal_rand_mbedtls.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_rand_mbedtls_nomain.o src/cryptoal_rand_mbedtls.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/cryptoal_rand_mbedtls.o ${OBJECTDIR}/src/cryptoal_rand_mbedtls_nomain.o;\
	fi

${OBJECTDIR}/src/cryptoal_rand_openssl_nomain.o: ${OBJECTDIR}/src/cryptoal_rand_openssl.o src/cryptoal_rand_openssl.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/cryptoal_rand_openssl.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_rand_openssl_nomain.o src/cryptoal_rand_openssl.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/cryptoal_rand_openssl.o ${OBJECTDIR}/src/cryptoal_rand_openssl_nomain.o;\
	fi

${OBJECTDIR}/src/cryptoal_signature_mbedtls_nomain.o: ${OBJECTDIR}/src/cryptoal_signature_mbedtls.o src/cryptoal_signature_mbedtls.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/cryptoal_signature_mbedtls.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_signature_mbedtls_nomain.o src/cryptoal_signature_mbedtls.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/cryptoal_signature_mbedtls.o ${OBJECTDIR}/src/cryptoal_signature_mbedtls_nomain.o;\
	fi

${OBJECTDIR}/src/cryptoal_signature_openssl_nomain.o: ${OBJECTDIR}/src/cryptoal_signature_openssl.o src/cryptoal_signature_openssl.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/cryptoal_signature_openssl.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_signature_openssl_nomain.o src/cryptoal_signature_openssl.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/cryptoal_signature_openssl.o ${OBJECTDIR}/src/cryptoal_signature_openssl_nomain.o;\
	fi

${OBJECTDIR}/src/cryptoal_tls_mbedtls_nomain.o: ${OBJECTDIR}/src/cryptoal_tls_mbedtls.o src/cryptoal_tls_mbedtls.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/cryptoal_tls_mbedtls.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_tls_mbedtls_nomain.o src/cryptoal_tls_mbedtls.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/cryptoal_tls_mbedtls.o ${OBJECTDIR}/src/cryptoal_tls_mbedtls_nomain.o;\
	fi

${OBJECTDIR}/src/cryptoal_tls_openssl_nomain.o: ${OBJECTDIR}/src/cryptoal_tls_openssl.o src/cryptoal_tls_openssl.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/cryptoal_tls_openssl.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -O2 -Iinc -Iplf/linux/inc -I../common/inc -I../nal/inc -I../nal/src/stor -I../nal/src/rtos -I../tools/inc -I../nal/plf/linux/inc -I../3-party/mbedtls/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cryptoal_tls_openssl_nomain.o src/cryptoal_tls_openssl.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/cryptoal_tls_openssl.o ${OBJECTDIR}/src/cryptoal_tls_openssl_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	    ${TESTDIR}/TestFiles/f5 || true; \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f4 || true; \
	    ${TESTDIR}/TestFiles/f6 || true; \
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
