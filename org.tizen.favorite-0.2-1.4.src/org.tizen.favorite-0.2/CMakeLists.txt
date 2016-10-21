#
#    Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT("favorite" C)

INCLUDE(FindPkgConfig)
pkg_check_modules(PKGS REQUIRED
		elementary
		capi-appfw-application
		app-utils
		pkgmgr-info
		capi-content-media-content
		glib-2.0
		capi-web-bookmark
		tv-service
		ui-gadget-1
		)

IF(NOT DEFINED PACKAGE_NAME)
	SET(PACKAGE_NAME "org.tizen.${PROJECT_NAME}")
ENDIF(NOT DEFINED PACKAGE_NAME)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})

IF(NOT DEFINED BINDIR)
	SET(BINDIR "${PREFIX}/bin")
ENDIF(NOT DEFINED BINDIR)
IF(NOT DEFINED RESDIR)
	SET(RESDIR "${PREFIX}/res")
ENDIF(NOT DEFINED RESDIR)
IF(NOT DEFINED EDJEDIR)
	SET(EDJEDIR "${PREFIX}/res/edje")
ENDIF(NOT DEFINED EDJEDIR)
IF(NOT DEFINED IMGDIR)
	SET(IMGDIR "${PREFIX}/res/images")
ENDIF(NOT DEFINED IMGDIR)
IF(NOT DEFINED PACKAGEDIR)
	SET(PACKAGEDIR "/usr/share/packages")
ENDIF(NOT DEFINED PACKAGEDIR)
IF(NOT DEFINED DESKTOP_ICONDIR)
	SET(DESKTOP_ICONDIR "/usr/share/icons")
ENDIF(NOT DEFINED DESKTOP_ICONDIR)
IF(NOT DEFINED DESKTOP_ICON)
	SET(DESKTOP_ICON ${PACKAGE_NAME}.png)
ENDIF(NOT DEFINED DESKTOP_ICON)

SET(SRCS
	src/main.c
	src/data/channel.c
	src/data/app.c
	src/data/media.c
	src/data/web.c
	src/common/utils.c
	src/view/view_base.c
	src/view/view_action_menu.c
	src/view/view_pin.c
	src/grid/grid_tv.c
	src/grid/grid_movie.c
	src/grid/grid_gallery.c
	src/grid/grid_music.c
	src/grid/grid_apps.c
	src/grid/grid_webs.c
   )

SET(TARGET_EDJ "${PROJECT_NAME}.edj")
SET(THEME_EDJ "${PROJECT_NAME}-theme.edj")

ADD_DEFINITIONS("-DPACKAGE=\"${PACKAGE_NAME}\"")
ADD_DEFINITIONS("-DEDJEDIR=\"${EDJEDIR}\"")
ADD_DEFINITIONS("-DEDJEFILE=\"${EDJEDIR}/${TARGET_EDJ}\"")
ADD_DEFINITIONS("-DTHEMEFILE=\"${EDJEDIR}/${THEME_EDJ}\"")
ADD_DEFINITIONS("-DIMGDIR=\"${IMGDIR}\"")

FOREACH(flag ${PKGS_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -Wall -Werror")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PKGS_LDFLAGS})

CONFIGURE_FILE(${PACKAGE_NAME}.xml.in ${PACKAGE_NAME}.xml)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${BINDIR})
INSTALL(FILES ${PACKAGE_NAME}.xml DESTINATION ${PACKAGEDIR})
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res/images/ DESTINATION
		${IMGDIR})
INSTALL(FILES ${CMAKE_SOURCE_DIR}/${DESKTOP_ICON} DESTINATION ${DESKTOP_ICONDIR})

ADD_SUBDIRECTORY(res)
