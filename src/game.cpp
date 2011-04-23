#include <pd/game.hpp>
#include <pd/main_menu.hpp>
#include <pd/utils.hpp>
#include <pd/config.hpp>
#include <pd/console.hpp>

/* carbon is needed for SetSystemUIMode which I don't know a nice replacement
   for in cocoa. */
#if PD_PLATFORM == PD_PLATFORM_OSX
#   include <Carbon/Carbon.h>
#endif


static const int window_width = 1280;
static const int window_height = 720;
static const pd::timedelta_t simulation_dt = 0.01f;

pd::game *pd::game::s_instance;


pd::game::game()
{
    assert(!s_instance);
    s_instance = this;

    PD_LOG("Initializing game");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        pd::critical_error("Could not initialize SDL", SDL_GetError());

    
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    
    SDL_Window *win = SDL_CreateWindow("Navitas",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!win)
        pd::critical_error("Unable to create draw() window", SDL_GetError());

    SDL_GLContext ctx = SDL_GL_CreateContext(win);
    SDL_GL_SetSwapInterval(1);

    m_win = win;
    m_glctx = ctx;
    on_resize();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

#if PD_PLATFORM == PD_PLATFORM_OSX
    m_osx_alternative_width = 0;
    m_osx_alternative_height = 0;
    m_fullscreen = false;
#endif
    m_screen = pd::main_menu::instance();
    m_running = true;
    m_last_delay = 0;
    m_console = new pd::console();
}

void pd::game::on_resize()
{
    int width, height;
    get_size(width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

pd::game::~game()
{
    s_instance = 0;
    delete m_console;
    SDL_GL_DeleteContext(m_glctx);
    SDL_DestroyWindow(m_win);
    SDL_Quit();
}

void pd::game::fullscreen(bool value)
{
    /* don't change what does not need change */
    if (value == m_fullscreen)
        return;

    /* on OS X fullscreen currently always means native resolution.  The
       screen_width and screen_height are just used in window mode.  The
       reasoning is that the native fullscreen mode in OS X does not
       allow tabbing out of the application which is super annoying. */
#if PD_PLATFORM == PD_PLATFORM_OSX
    int flags = SDL_GetWindowFlags(m_win);
    SDL_Window *new_win;
    if (value) {
        /* we cannot toggle borderless and regular window mode, so we have
           to recreate the window.  Sucks but well but works. */
        SDL_DisplayMode mode;
        SDL_GetDesktopDisplayMode(SDL_GetWindowDisplay(m_win), &mode);
        new_win = SDL_CreateWindow(SDL_GetWindowTitle(m_win),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            mode.w, mode.h,
            flags | SDL_WINDOW_BORDERLESS);
        SDL_GetWindowDisplayMode(m_win, &mode);
        m_osx_alternative_width = (size_t)mode.w;
        m_osx_alternative_height = (size_t)mode.h;
    } else {
        /* same thing, different direction */
        new_win = SDL_CreateWindow(SDL_GetWindowTitle(m_win),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            (int)m_osx_alternative_width, (int)m_osx_alternative_height,
            flags & ~SDL_WINDOW_BORDERLESS);
    }
    SDL_DestroyWindow(m_win);
    m_win = new_win;
    SetSystemUIMode(value ? kUIModeAllHidden : kUIModeNormal, 0);
    SDL_GL_MakeCurrent(m_win, m_glctx);
    on_resize();
#else
    SDL_SetWindowFullscreen(m_win, value ? SDL_TRUE : SDL_FALSE);
#endif

    m_fullscreen = value;
}

void pd::game::swap()
{
    glFlush();
    SDL_GL_SwapWindow(m_win);
}

void pd::game::run()
{
    SDL_Event evt;
    uint64_t old_ticks = 0;
    pd::timedelta_t accumulator = 0.0f;

    while (m_running) {
        uint64_t now = pd::get_ticks();
        pd::timedelta_t dt = (now - old_ticks) / pd::get_tick_frequency();
        old_ticks = now;

        // this took way too long.  We probably had a suspended mainloop
        // (debugger, window moved etc.)
        if (dt > 0.1f)
            dt = simulation_dt;

        while (SDL_PollEvent(&evt))
            handle_event(evt);

        // run the simulation at a fixed frequency.
        accumulator += dt;
        while (accumulator >= simulation_dt) {
            update(simulation_dt);
            accumulator -= simulation_dt;
        }

        draw();
        swap();

        // wait a little bit so that our timer has enough precision
        pd::delay(1);
    }
}

void pd::game::update(pd::timedelta_t dt)
{
    if (m_screen)
        m_screen->update(dt);
}

void pd::game::draw() const
{
    if (m_screen)
        m_screen->draw();

    if (m_console->visible())
        m_console->draw();
}

void pd::game::handle_event(SDL_Event &evt)
{
    if (evt.type == SDL_QUIT)
        stop();
    else if (evt.type == SDL_KEYDOWN) {
        switch (evt.key.keysym.sym) {
        case SDLK_F1:
            PD_LOG("Reloading config files");
            pd::config::load();
            break;
        case SDLK_f:
            toggle_fullscreen();
            break;
        case SDLK_BACKQUOTE:
            m_console->toggle_visibility();
            break;
        }
    }
    if (m_screen)
        m_screen->handle_event(evt);
}
