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
include_directories(${SDL2_INCLUDE_DIRS})


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

message( "!!!+++++++ ${CMAKE_CURRENT_SOURCE_DIR}/SfmlLib/SFML/include/SFML/ ")
set(${SFML_INCLUDE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/SfmlLib/SFML/include/SFML/")
message( "++++++++++ ${SFML_INCLUDE_DIR} ")
find_package(SFML COMPONENTS system window graphics REQUIRED )
if (SFML_FOUND)
    message( "++++++++++ ${SFML_INCLUDE_DIR} ")
	include_directories(${SFML_INCLUDE_DIR})
else()
    message("/n -> SFML not found")
endif()


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

file(GLOB SfmlLibSources
	${CMAKE_CURRENT_SOURCE_DIR}/SfmlLib/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SfmlLib/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/IWindow/*.hpp)

add_library(SfmlLib SHARED ${SfmlLibSources})
target_link_libraries(SfmlLib ${SFML_LIBRARIES})

file(GLOB TesterSources
	${CMAKE_CURRENT_SOURCE_DIR}/Tester/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Tester/*.hpp)

add_executable(Nibbler main.cpp)

add_executable(Tester ${TesterSources})

target_link_libraries(SdlLib ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

target_link_libraries(NcursesLib ncurses)

target_link_libraries(MyLogicLib NcursesLib DummyLib SdlLib SfmlLib)

target_link_libraries(Nibbler MyLogicLib)

target_link_libraries(Tester MyLogicLib)
