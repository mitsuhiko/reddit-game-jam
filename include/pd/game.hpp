#ifndef _INC_PD_GAME_HPP_
#define _INC_PD_GAME_HPP_
#include <pd/pd.hpp>

namespace pd {

    class game {
    public:
        game();
        ~game();

        SDL_Window *win() { return m_win; }
        SDL_GLContext glctx() { return m_glctx; }

        void run();
        void stop() { m_running = false; }
        bool is_running() const { return m_running; }

        void handle_event(SDL_Event &evt, float dt);
        void update(float dt);
        void render(float dt) const;

    private:
        SDL_Window *m_win;
        SDL_GLContext m_glctx;
        bool m_running;
        unsigned int m_start_time;
        unsigned int m_end_time;
    };
}

#endif
