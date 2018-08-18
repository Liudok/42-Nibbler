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

set(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".dylib")


find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
#include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#find_package(SFML REQUIRED system window graphics)
#if (SFML_FOUND)
#    include_directories(${SFML_INCLUDE_DIR})
#    target_link_libraries(SFMLDemo ${SFML_LIBRARIES})
#endif()

project(Nibbler)

file(GLOB LogicSources
	${CMAKE_CURRENT_SOURCE_DIR}/LogicLib/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LogicLib/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/IWindow/*.hpp)

add_library(MyLogicLib STATIC ${LogicSources})

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

file(GLOB TesterSources
	${CMAKE_CURRENT_SOURCE_DIR}/Tester/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Tester/*.hpp)

add_executable(Nibbler main.cpp)

add_executable(Tester ${TesterSources})

target_link_libraries(SdlLib ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

target_link_libraries(NcursesLib ncurses)

target_link_libraries(MyLogicLib NcursesLib DummyLib SdlLib)

target_link_libraries(Nibbler MyLogicLib)

target_link_libraries(Tester MyLogicLib)
