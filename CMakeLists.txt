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

# Glfw configuration

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(GlfwPath ${CMAKE_CURRENT_SOURCE_DIR}/NibblerThirdParties/GLFW)
add_subdirectory(${GlfwPath})

# Project sources and libraries

project(Nibbler)

file(GLOB LogicSources
        ${CMAKE_CURRENT_SOURCE_DIR}/LogicLib/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LogicLib/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/IWindow/*.hpp)

add_library(LogicLib STATIC ${LogicSources})


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

file(GLOB GlfwLibSources
        ${CMAKE_CURRENT_SOURCE_DIR}/GlfwLib/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GlfwLib/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/IWindow/*.hpp)

add_library(GlfwLib SHARED ${GlfwLibSources})

add_executable(Nibbler main.cpp)

# Simpler includes

target_include_directories(LogicLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)
target_include_directories(SdlLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)
target_include_directories(SfmlLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)
target_include_directories(GlfwLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/IWindow)

# Links between project parts

target_link_libraries(LogicLib SdlLib SfmlLib GlfwLib)
target_link_libraries(SdlLib ${SdlDynamicLibs})
target_link_libraries(SfmlLib ${SfmlDynamicLibs})
target_link_libraries(GlfwLib glfw "-framework OpenGL")
target_link_libraries(Nibbler LogicLib)
