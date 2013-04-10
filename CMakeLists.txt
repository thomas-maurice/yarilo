CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
include(ExternalProject)

PROJECT(Yarilo)

FIND_PACKAGE(OpenSSL)
FIND_PACKAGE(Lua5)

IF(NOT OPENSSL_FOUND)
	message(FATAL_ERROR "You need to install openssl. Exiting.")
ENDIF()

IF(WIN32)
	LIST(APPEND YARILO_LIBS "libyarilo_static")
	LIST(APPEND YARILO_LIBS "plugin")
	LIST(APPEND YARILO_LIBS "tsocket")
	LIST(APPEND YARILO_LIBS "${OPENSSL_LIBRARIES}")
	LIST(APPEND YARILO_LIBS "wsock32")
ELSE()
	SET(YARILO_LIBS "")
ENDIF()

SET(CMAKE_C_FLAGS "-O2 -s -DSTDC_HEADERS=1 -DHAVE_STRING_H=1 -DHAVE_ALLOCA_H=1")
SET(CMAKE_CXX_FLAGS "-Wall -O2 -s")
SET(LIBRARY_OUTPUT_PATH
	${PROJECT_BINARY_DIR}/lib
)

INCLUDE_DIRECTORIES("include"
	"${PROJECT_BINARY_DIR}/libplugin/include" 
	"${PROJECT_BINARY_DIR}/libplugin/include/libplugin"
	"${PROJECT_BINARY_DIR}/libtsocket/include"  
	"${PROJECT_BINARY_DIR}/libtsocket/include/libtsocket"
	"${LIBRARY_OUTPUT_PATH}"
	"${OPENSSL_INCLUDE_DIR}"
)  

FILE(
	GLOB_RECURSE
	sources_yarilo
	src/*.cpp
	src/*.c
	include/*.h
)

ADD_LIBRARY(
	Yarilo_common
	OBJECT
	${sources_yarilo}
)

ADD_EXECUTABLE(
	Yarilo
	$<TARGET_OBJECTS:Yarilo_common>
)

ADD_LIBRARY(
	libyarilo_static
	STATIC
	$<TARGET_OBJECTS:Yarilo_common>
)

ADD_LIBRARY(
	libyarilo_shared
	SHARED
	$<TARGET_OBJECTS:Yarilo_common>
)

SET_TARGET_PROPERTIES(
	libyarilo_static
	PROPERTIES
	PREFIX ""
	OUTPUT_NAME "libyarilo"
)

SET_TARGET_PROPERTIES(
	libyarilo_shared
	PROPERTIES
	PREFIX ""
	OUTPUT_NAME "libyarilo"
)

ExternalProject_Add(libplugin
   PREFIX libplugin
   GIT_REPOSITORY http://github.com/svartbergtroll/libplugin
   CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/libplugin"
   INSTALL_DIR "${PROJECT_BINARY_DIR}/libplugin"
)

ExternalProject_Add(libtsocket
   PREFIX libtsocket
   GIT_REPOSITORY http://github.com/svartbergtroll/libtsocket
   CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/libtsocket"
   INSTALL_DIR "${PROJECT_BINARY_DIR}/libtsocket"
)

ADD_LIBRARY(plugin STATIC IMPORTED)
SET_TARGET_PROPERTIES(plugin
	PROPERTIES
	IMPORTED_LOCATION ${PROJECT_BINARY_DIR}/libplugin/lib/libplugin.a
)

ADD_LIBRARY(tsocket STATIC IMPORTED)
SET_TARGET_PROPERTIES(tsocket
	PROPERTIES
	IMPORTED_LOCATION ${PROJECT_BINARY_DIR}/libtsocket/lib/libtsocket.a
)

IF(WIN32)
TARGET_LINK_LIBRARIES(
	Yarilo
	plugin
  tsocket
	${OPENSSL_LIBRARIES}
  wsock32
)
TARGET_LINK_LIBRARIES(
	libyarilo_shared
	plugin
  tsocket
	${OPENSSL_LIBRARIES}
  wsock32
)
ELSE()
TARGET_LINK_LIBRARIES(
	Yarilo
	plugin
	dl
  tsocket
	${OPENSSL_LIBRARIES}
)
ENDIF()

ADD_DEPENDENCIES(
	Yarilo_common
	DEPENDS
	libplugin)

ADD_DEPENDENCIES(
	Yarilo_common
	DEPENDS
	libtsocket)

INSTALL(TARGETS Yarilo DESTINATION .)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/conf DESTINATION .)

ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/modules ${PROJECT_BINARY_DIR}/modules)
