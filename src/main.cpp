#define PD_MAGIC_MAIN
#include <pd/pd.hpp>
#include <pd/path.hpp>
#include <pd/game.hpp>

#if PD_PLATFORM == PD_PLATFORM_WINDOWS
#pragma comment(lib, "shlwapi")
#include "shlobj.h"
#include "shlwapi.h"

static void initialize_win32_console(size_t width, size_t height,
                                     size_t scrollback)
{
    int handle;
    COORD buffer_size = {width, scrollback};
    SMALL_RECT window_size = {0, 0, width - 1, height};
    AllocConsole();
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), buffer_size);
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &window_size);
    handle = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
    *stdout = *_fdopen(handle, "w");
    handle = _open_osfhandle((long)GetStdHandle(STD_ERROR_HANDLE), _O_TEXT);
    *stderr = *_fdopen(handle, "w");
    handle = _open_osfhandle((long)GetStdHandle(STD_INPUT_HANDLE), _O_TEXT);
    *stdin = *_fdopen(handle, "r");
    SetConsoleTitleW(L"Debug Output");
}
#endif


int main(int argc, char **argv)
{
    /* on windows we have to initialize the console window */
#if PD_PLATFORM == PD_PLATFORM_WINDOWS && !defined(NDEBUG)
    initialize_win32_console(120, 30, 1000);
#endif

    /* we switch to the resource folder before the engine starts up so
       that game and engine can reference things in the resource folder */
    std::string resource_path = pd::path::get_resource_path();
    pd::path::set_cwd(resource_path);

    pd::game game;
    game.run();

    return 0;
}
