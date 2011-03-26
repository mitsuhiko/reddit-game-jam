cmake_minimum_required(VERSION 2.6)

project(ProbablyDefective)

set(GAME_SOURCES
	src/main.cpp
	src/path.cpp
	src/game.cpp
    src/image.cpp
    src/texture.cpp
	src/drawtools.cpp
	src/sprite.cpp
    src/font.cpp
	src/map.cpp
    src/resource_manager.cpp

	src/screen.cpp
	src/main_menu.cpp
)
set(GAME_HEADERS
	include/pd/pd.hpp
	include/pd/path.hpp
	include/pd/game.hpp
	include/pd/image.hpp
	include/pd/texture.hpp
	include/pd/math.hpp
	include/pd/drawtools.hpp
	include/pd/color.hpp
	include/pd/sprite.hpp
    include/pd/font.hpp
	include/pd/lexical_cast.hpp
	include/pd/map.hpp
	include/pd/resource_base.hpp
	include/pd/resource_manager.hpp

	include/pd/screen.hpp
	include/pd/main_menu.hpp
)

if(APPLE)
	set(SDL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/sdl-1.3/local)
	set(SDL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/libs/sdl-1.3/local/include/SDL)
	set(SDL_MAIN_PATH ${SDL_PATH})
elseif(WIN32)
	set(SDL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/sdl-1.3/VisualC/SDL/Release)
	set(SDL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/libs/sdl-1.3/include)
	set(SDL_MAIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/sdl-1.3/VisualC/SDLmain/Release)
endif()
set(GLEW_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-1.5.8)

find_library(SDL_LIBRARY
	NAMES SDL-1.3.0 SDL
	PATHS ${SDL_PATH}
	PATH_SUFFIXES lib
	NO_DEFAULT_PATH
)

find_library(SDLMAIN_LIBRARY
	NAMES SDLmain
	PATHS ${SDL_MAIN_PATH}
	PATH_SUFFIXES lib
	NO_DEFAULT_PATH
)

find_package(OpenGL REQUIRED)

set(EXTRA_LIBS "")
if(APPLE)
    set(CMAKE_C_FLAGS "-g -Wall")
    set(CMAKE_CXX_FLAGS "-g -Wall")
	set(EXTRA_LIBS ${EXTRA_LIBS} "-framework Cocoa -framework Carbon")
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
	${SDL_INCLUDE}
	${CMAKE_CURRENT_SOURCE_DIR}/libs/vendor
	${CMAKE_CURRENT_SOURCE_DIR}/libs/vendor/utfcpp
	${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype-2.4.4/include
)

add_executable(
	ProbablyDefective
	WIN32
	MACOSX_BUNDLE
    ${GAME_SOURCES}
    ${GAME_HEADERS}
)

target_link_libraries(
	ProbablyDefective
	${OPENGL_LIBRARIES}
	${SDL_LIBRARY}
	${SDLMAIN_LIBRARY}
	${FREETYPE_LIBRARY}
    ${GLEW_LIBRARY}
	${EXTRA_LIBS}
)

if(APPLE)
	set(BUNDLE_BINARY
		${CMAKE_CURRENT_BINARY_DIR}/ProbablyDefective.app/Contents/MacOS/ProbablyDefective)

	add_custom_command(TARGET ProbablyDefective POST_BUILD
		COMMAND python scripts/frameworkify.py
			${BUNDLE_BINARY}
			${SDL_LIBRARY}
	)

	add_custom_command(TARGET ProbablyDefective POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "resources"
			${CMAKE_CURRENT_BINARY_DIR}/ProbablyDefective.app/Contents/Resources)
elseif(WIN32)
	set(VS_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
	add_custom_command(TARGET ProbablyDefective POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL_PATH}/SDL.dll
			${VS_OUTPUT_DIR}/SDL.dll
	)
endif()
