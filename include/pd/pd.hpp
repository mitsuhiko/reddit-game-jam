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

/* glm */
#include <glm/glm.hpp>

/* logging */
#define PD_LOG(Expr) do { \
    std::string _filename(__FILE__); \
    size_t pos = _filename.find_last_of("\\/"); \
    if (pos != std::string::npos) \
        _filename = _filename.substr(pos + 1); \
    std::clog << "[" << _filename << "] " << Expr << std::endl; \
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

    /* the type for timedeltas */
    typedef float timedelta_t;
    
    /* use glm vectors for the time being.  I don't plan on replacing them
       anytime soon, but maybe something better comes around so better be
       prepaired. */
    using glm::vec2;
    using glm::mat2;
    using glm::mat3;
}

#endif
