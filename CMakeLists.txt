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

# Sdl configuration

set(Sdl2Path ${CMAKE_CURRENT_SOURCE_DIR}/NibblerThirdParties/SDL2.framework)
set(Sdl2TtfPath ${CMAKE_CURRENT_SOURCE_DIR}/NibblerThirdParties/SDL2_ttf.framework)
include_directories(${Sdl2Path}/Headers ${Sdl2TtfPath}/Headers)
file(GLOB SdlDynamicLibs ${Sdl2Path}/SDL2 ${Sdl2TtfPath}/SDL2_ttf)

# Sfml configuration

set(SfmlPath ${CMAKE_CURRENT_SOURCE_DIR}/NibblerThirdParties/SFML)
include_directories(${SfmlPath}/include)
file(GLOB SfmlDynamicLibs ${SfmlPath}/lib/*.dylib)

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

add_executable(Nibbler main.cpp)

# Simpler includes

target_include_directories(LogicLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)
target_include_directories(NcursesLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)
target_include_directories(SdlLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)
target_include_directories(SfmlLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)

# Links between project parts

target_link_libraries(LogicLib NcursesLib SdlLib SfmlLib)
target_link_libraries(NcursesLib ncurses)
target_link_libraries(SdlLib ${SdlDynamicLibs})
target_link_libraries(SfmlLib ${SfmlDynamicLibs})
target_link_libraries(Nibbler LogicLib)
