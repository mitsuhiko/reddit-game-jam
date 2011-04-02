#ifndef _INC_PD_GAME_HPP_
#define _INC_PD_GAME_HPP_
#include <pd/pd.hpp>
#include <pd/resource_manager.hpp>

namespace pd {

    class screen;

    class game {
    public:
        game();
        ~game();

        SDL_Window *win() { return m_win; }
        SDL_GLContext glctx() { return m_glctx; }

        void run();
        void stop() { m_running = false; }
        bool is_running() const { return m_running; }

        void handle_event(SDL_Event &evt, pd::timedelta_t dt);
        void update(pd::timedelta_t dt);
        void render(pd::timedelta_t dt) const;
        void swap(pd::timedelta_t dt);

        pd::screen *screen() { return m_screen; }
        void screen(pd::screen *val) { m_screen = val; }
        static pd::game &instance() { return *s_instance; }

        void get_size(int &width, int &height) const { return SDL_GetWindowSize(m_win, &width, &height); }
        int width() const { int width, height; get_size(width, height); return width; }
        int height() const { int width, height; get_size(width, height); return height; }

        pd::resource_manager &resmgr() { return m_resmgr; }

    private:
        SDL_Window *m_win;
        SDL_GLContext m_glctx;
        pd::screen *m_screen;
        bool m_running;
        pd::resource_manager m_resmgr;
        static pd::game *s_instance;
        uint64_t m_last_delay;
    };


    /* shortcut for the lazy */
    template <class T>
    T *get_resource(const std::string &filename)
    {
        return game::instance().resmgr().get<T>(filename);
    }
}

#endif
