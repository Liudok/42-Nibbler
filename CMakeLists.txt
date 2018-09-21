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

# Third parties configuration

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wno-deprecated")
set(NibblerThirdPartiesPath ${CMAKE_CURRENT_SOURCE_DIR}/NibblerThirdParties)
add_definitions(-DPATH_TO_THIRD_PARTIES="${NibblerThirdPartiesPath}")

# Sdl configuration

set(Sdl2Path ${NibblerThirdPartiesPath}/SDL2.framework)
set(Sdl2TtfPath ${NibblerThirdPartiesPath}/SDL2_ttf.framework)
include_directories(${Sdl2Path}/Headers ${Sdl2TtfPath}/Headers)
file(GLOB SdlDynamicLibs ${Sdl2Path}/SDL2 ${Sdl2TtfPath}/SDL2_ttf)

# Sfml configuration

set(SfmlPath ${NibblerThirdPartiesPath}/SFML)
include_directories(${SfmlPath}/include)
file(GLOB SfmlDynamicLibs ${SfmlPath}/lib/*.dylib)

# Glfw configuration

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(GlfwPath ${NibblerThirdPartiesPath}/GLFW)
add_subdirectory(${GlfwPath})

# Project sources and libraries configuration

# Nibbler

project(Nibbler)

add_executable(Nibbler main.cpp)

# Logic

file(GLOB LogicSources
        ${CMAKE_CURRENT_SOURCE_DIR}/LogicLib/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LogicLib/*.hpp)

add_library(LogicLib STATIC ${LogicSources})

# GlfwLib

file(GLOB GlfwLibSources
        ${CMAKE_CURRENT_SOURCE_DIR}/GlfwLib/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GlfwLib/*.hpp)

add_library(GlfwLib SHARED ${GlfwLibSources})

# SdlLib

file(GLOB SdlLibSources
        ${CMAKE_CURRENT_SOURCE_DIR}/SdlLib/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SdlLib/*.hpp)

add_library(SdlLib SHARED ${SdlLibSources})

# SfmlLib

file(GLOB SfmlLibSources
        ${CMAKE_CURRENT_SOURCE_DIR}/SfmlLib/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SfmlLib/*.hpp)

add_library(SfmlLib SHARED ${SfmlLibSources})

# SoundLib

file(GLOB SoundLibSources
        ${CMAKE_CURRENT_SOURCE_DIR}/SoundLib/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SoundLib/*.hpp)

add_library(SoundLib SHARED ${SoundLibSources})

# Simpler includes

target_include_directories(LogicLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GlfwLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(SdlLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(SfmlLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(SoundLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Links between project parts

target_link_libraries(GlfwLib glfw "-framework OpenGL -framework GLUT")
target_link_libraries(SdlLib ${SdlDynamicLibs})
target_link_libraries(SfmlLib ${SfmlDynamicLibs})
target_link_libraries(SoundLib ${SfmlDynamicLibs})
target_link_libraries(Nibbler LogicLib)
