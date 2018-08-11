cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "In-source builds are prohibited. Sorry :(")
endif()

if (NOT APPLE)
	message(FATAL_ERROR
	"The project may be built only in macOS for now. Sorry :(")
endif()

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")

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

file(GLOB TesterSources
	${CMAKE_CURRENT_SOURCE_DIR}/Tester/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Tester/*.hpp)

add_executable(Nibbler main.cpp)

add_executable(Tester ${TesterSources})

target_link_libraries(NcursesLib ncurses)

target_link_libraries(MyLogicLib NcursesLib DummyLib)

target_link_libraries(Nibbler MyLogicLib)

target_link_libraries(Tester MyLogicLib)
