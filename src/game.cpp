#include <pd/game.hpp>
#include <pd/main_menu.hpp>

const int width = 800;
const int height = 450;

pd::game *pd::game::s_instance;


pd::game::game()
{
    assert(!s_instance);
    s_instance = this;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        pd::critical_error("Could not initialize SDL", SDL_GetError());
    
    SDL_Window *win = SDL_CreateWindow("Working Title",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!win)
        pd::critical_error("Unable to create render window", SDL_GetError());

    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    SDL_GLContext ctx = SDL_GL_CreateContext(win);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 800.0f, 450.0f, 0.0f, 0.0f, 1000.0f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    m_win = win;
    m_glctx = ctx;
    m_screen = pd::main_menu::instance();
    m_running = true;
}

pd::game::~game()
{
    s_instance = 0;
    SDL_GL_DeleteContext(m_glctx);
    SDL_DestroyWindow(m_win);
    SDL_Quit();
}

void pd::game::run()
{
    SDL_Event evt;
    while (m_running) {
        unsigned int old_start = m_start_time;
        m_start_time = SDL_GetTicks();
        float dt = (m_end_time - old_start) / 1000.0f;

        while (SDL_PollEvent(&evt))
            handle_event(evt, dt);
        update(dt);
        render(dt);

        SDL_GL_SwapWindow(m_win);
        m_end_time = SDL_GetTicks();
    }
}

void pd::game::update(float dt)
{
    if (m_screen)
        m_screen->update(dt);
}

void pd::game::render(float dt) const
{
    if (m_screen)
        m_screen->render(dt);
}

void pd::game::handle_event(SDL_Event &evt, float dt)
{
    if (evt.type == SDL_QUIT)
        stop();
    if (m_screen)
        m_screen->handle_event(evt, dt);
}