# General requirements

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "In-source builds are prohibited. Sorry :(")
endif()

if (NOT APPLE)
	message(FATAL_ERROR
	"The project may be built only in macOS for now. Sorry :(")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")

# Sdl2 configuration

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Sfml configuration

include_directories(${SFML2_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/SFML/include)
include_directories(${SFML2_LIB_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib)

# Project sources and libraries

project(Nibbler)

file(GLOB LogicSources
	${CMAKE_CURRENT_SOURCE_DIR}/LogicLib/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LogicLib/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/IWindow/*.hpp)

add_library(LogicLib STATIC ${LogicSources})

file(GLOB NcursesLibSources
	${CMAKE_CURRENT_SOURCE_DIR}/NcursesLib/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/NcursesLib/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/IWindow/*.hpp)

add_library(NcursesLib SHARED ${NcursesLibSources})

file(GLOB DummyLibSources
	${CMAKE_CURRENT_SOURCE_DIR}/DummyLib/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DummyLib/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/IWindow/*.hpp)

add_library(DummyLib SHARED ${DummyLibSources})

file(GLOB SdlLibSources
	${CMAKE_CURRENT_SOURCE_DIR}/SdlLib/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SdlLib/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/IWindow/*.hpp)

add_library(SdlLib SHARED ${SdlLibSources})

file(GLOB SfmlLibSources
	${CMAKE_CURRENT_SOURCE_DIR}/SfmlLib/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SfmlLib/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/IWindow/*.hpp)

add_library(SfmlLib SHARED ${SfmlLibSources})

file(GLOB SfmlDynamicLibs ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/*.dylib)

add_executable(Nibbler main.cpp)

# Simpler includes

target_include_directories(LogicLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)
target_include_directories(NcursesLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)
target_include_directories(DummyLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)
target_include_directories(SdlLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)
target_include_directories(SfmlLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)

# Links between project parts

target_link_libraries(LogicLib NcursesLib DummyLib SdlLib SfmlLib)

target_link_libraries(NcursesLib ncurses)

target_link_libraries(SdlLib ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

target_link_libraries(SfmlLib ${SfmlDynamicLibs})

target_link_libraries(Nibbler LogicLib)
