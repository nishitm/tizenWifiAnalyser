CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(fw_name "capi-network-wifi")

PROJECT(${fw_name})

SET(PREFIX ${CMAKE_INSTALL_PREFIX})

SET(LIB ${LIB_PATH})
SET(LIBDIR ${PREFIX}/${LIB_PATH})

SET(INC_DIR include)
INCLUDE_DIRECTORIES(${INC_DIR})

SET(dependents "dlog vconf capi-appfw-application capi-base-common capi-system-info glib-2.0 gio-2.0 gthread-2.0 network ")
SET(pc_dependents "capi-base-common")

IF(TIZEN_DUALSIM_ENABLE)
    ADD_DEFINITIONS(-DTIZEN_DUALSIM_ENABLE)
ENDIF(TIZEN_DUALSIM_ENABLE)
IF(TIZEN_WEARABLE)
	ADD_DEFINITIONS(-DTIZEN_WEARABLE)
ENDIF(TIZEN_WEARABLE)
IF(TIZEN_MOBILE)
	ADD_DEFINITIONS(-DTIZEN_MOBILE)
ENDIF(TIZEN_MOBILE)

IF(TIZEN_TV)
	ADD_DEFINITIONS(-DTIZEN_TV)
ENDIF(TIZEN_TV)

INCLUDE(FindPkgConfig)
pkg_check_modules(${fw_name} REQUIRED ${dependents} ${APPFW_REQUIRED_PKGS})
FOREACH(flag ${${fw_name}_CFLAGS})
    SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -fPIC -Wall -Werror -fvisibility=hidden")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -fPIC -Wall -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g")

IF("${ARCH}" STREQUAL "arm")
    ADD_DEFINITIONS("-DTARGET")
ENDIF("${ARCH}" STREQUAL "arm")

ADD_DEFINITIONS("-DPREFIX=\"${CMAKE_INSTALL_PREFIX}\"")
ADD_DEFINITIONS("-DTIZEN_DEBUG")

SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed -Wl,--rpath=${LIBDIR}")

aux_source_directory(src SOURCES)
ADD_LIBRARY(${fw_name} SHARED ${SOURCES})

TARGET_LINK_LIBRARIES(${fw_name} ${${fw_name}_LDFLAGS})

SET_TARGET_PROPERTIES(${fw_name}
     PROPERTIES
     VERSION ${FULLVER}
     SOVERSION ${MAJORVER}
     CLEAN_DIRECT_OUTPUT 1
)

INSTALL(TARGETS ${fw_name} DESTINATION ${LIB})
INSTALL(
        DIRECTORY ${INC_DIR}/ DESTINATION include/network
        FILES_MATCHING
        PATTERN "*_private.h" EXCLUDE
        PATTERN "${INC_DIR}/*.h"
        )

SET(PC_NAME ${fw_name})
SET(PC_REQUIRED ${pc_dependents})
SET(PC_LDFLAGS -l${fw_name})

CONFIGURE_FILE(
    ${fw_name}.pc.in
    ${CMAKE_CURRENT_SOURCE_DIR}/${fw_name}.pc
    @ONLY
)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${fw_name}.pc DESTINATION ${LIB}/pkgconfig)

ADD_SUBDIRECTORY(test)

IF(UNIX)

ADD_CUSTOM_TARGET (distclean @echo cleaning for source distribution)
ADD_CUSTOM_COMMAND(
        DEPENDS clean
        COMMENT "distribution clean"
        COMMAND find
        ARGS    .
        -not -name config.cmake -and \(
        -name tester.c -or
        -name Testing -or
        -name CMakeFiles -or
        -name cmake.depends -or
        -name cmake.check_depends -or
        -name CMakeCache.txt -or
        -name cmake.check_cache -or
        -name *.cmake -or
        -name Makefile -or
        -name core -or
        -name core.* -or
        -name gmon.out -or
        -name install_manifest.txt -or
        -name *.pc -or
        -name *~ \)
        | grep -v TC | xargs rm -rf
        TARGET  distclean
        VERBATIM
)

ENDIF(UNIX)
