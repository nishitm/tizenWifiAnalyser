ADD_CUSTOM_TARGET(${TARGET_EDJ}
		COMMAND edje_cc -id images
		${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.edc
		${CMAKE_CURRENT_BINARY_DIR}/${TARGET_EDJ}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.edc
)
ADD_DEPENDENCIES(${PROJECT_NAME} ${TARGET_EDJ})
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_EDJ} DESTINATION ${EDJEDIR})

ADD_CUSTOM_TARGET(${THEME_EDJ}
		COMMAND edje_cc -id images -sd sounds
		${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}-theme.edc
		${CMAKE_CURRENT_BINARY_DIR}/${THEME_EDJ}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}-theme.edc
)
ADD_DEPENDENCIES(${PROJECT_NAME} ${THEME_EDJ})
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${THEME_EDJ} DESTINATION ${EDJEDIR})
