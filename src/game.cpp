#include <pd/game.hpp>
#include <pd/main_menu.hpp>
#include <pd/utils.hpp>
#include <pd/config.hpp>

static const int window_width = 1280;
static const int window_height = 720;
static const pd::timedelta_t simulation_dt = 0.016;

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
        pd::critical_error("Unable to create render window", SDL_GetError());

    SDL_GLContext ctx = SDL_GL_CreateContext(win);
    SDL_GL_SetSwapInterval(1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, window_width, window_height, 0.0f, 0.0f, 1000.0f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    m_win = win;
    m_glctx = ctx;
    m_screen = pd::main_menu::instance();
    m_running = true;
    m_last_delay = 0;
}

pd::game::~game()
{
    s_instance = 0;
    SDL_GL_DeleteContext(m_glctx);
    SDL_DestroyWindow(m_win);
    SDL_Quit();
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

        render(dt);
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

void pd::game::render(pd::timedelta_t dt) const
{
    if (m_screen)
        m_screen->render(dt);
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
        }
    }
    if (m_screen)
        m_screen->handle_event(evt);
}
