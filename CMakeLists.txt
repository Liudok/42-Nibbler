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

set(SdlPath ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParties/SDL)
<<<<<<< HEAD
include_directories(${SDL_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParties/SDL/sdl2frameworks/SDL2.framework/Headers ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParties/SDL/sdl2frameworks/SDL2_ttf.framework/Headers ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParties/SDL/sdl2frameworks/SDL2_ttf.framework ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParties/SDL/sdl2frameworks/SDL2.framework )
file(GLOB SdlDynamicLibs ${SdlPath}/lib/*.dylib ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParties/SDL/sdl2frameworks/SDL2_ttf.framework/SDL2_ttf)
=======
include_directories(${SDL_INCLUDE_DIRS} ${SdlPath}/include)
file(GLOB SdlDynamicLibs ${SdlPath}/lib/*.dylib)
>>>>>>> master

# Sfml configuration

set(SfmlPath ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParties/SFML)
include_directories(${SFML2_INCLUDE_DIRS} ${SfmlPath}/include)
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
<<<<<<< HEAD
=======
target_include_directories(DummyLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)
>>>>>>> master
target_include_directories(SdlLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)
target_include_directories(SfmlLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)

# Links between project parts

<<<<<<< HEAD
target_link_libraries(LogicLib NcursesLib SdlLib SfmlLib)
=======
target_link_libraries(LogicLib NcursesLib DummyLib SdlLib SfmlLib)
>>>>>>> master

target_link_libraries(NcursesLib ncurses)

target_link_libraries(SdlLib ${SdlDynamicLibs})

target_link_libraries(SfmlLib ${SfmlDynamicLibs})

<<<<<<< HEAD
=======
target_link_libraries(Nibbler LogicLib)
>>>>>>> master
