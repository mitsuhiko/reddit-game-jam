#ifndef _INC_PD_PD_HPP_
#define _INC_PD_PD_HPP_

/* platform identifiers */
#define PD_PLATFORM_OSX         1
#define PD_PLATFORM_WINDOWS     2

/* figure out current platform */
#if defined(__APPLE__) && defined(__MACH__)
#  define PD_PLATFORM PD_PLATFORM_OSX
#elif defined(WIN32) || defined(_WINDOWS)
#  define _CRT_SECURE_NO_WARNINGS
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <windows.h>
#  include <fcntl.h>
#  include <io.h>
#  include <conio.h>
#  define PD_PLATFORM PD_PLATFORM_WINDOWS
#  pragma warning(disable: 4355)
#else
#  error "Current platform not supported"
#endif

/* things we always want to have arround. */
#include <new>
#include <string>
#include <cassert>
#include <iostream>

/* we also include SDL by default */
#include <SDL.h>
#include <SDL_opengl.h>
#ifndef PD_MAGIC_MAIN
#  undef main
#endif

/* same goes with box2d */
#include <Box2D/Box2D.h>

/* logging */
#define PD_LOG(Comp, Expr) do { \
    std::clog << "[" << Comp << "] " << Expr << std::endl; \
} while (0)

/* if things go south. */
namespace pd {

    inline void critical_error(const std::string &title, const std::string &text)
    {
#if PD_PLATFORM == PD_PLATFORM_WINDOWS
        MessageBoxA(NULL, text.c_str(), title.c_str(),
            MB_OK | MB_SETFOREGROUND | MB_ICONSTOP);
#else
        std::cout << "Critical error: " << title << std::endl << text << std::endl;
#endif
        ::exit(1);
    }

}

#endif
