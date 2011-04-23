#ifndef _INC_PD_GAME_HPP_
#define _INC_PD_GAME_HPP_
#include <pd/pd.hpp>
#include <pd/resource_manager.hpp>


namespace pd {

    class screen;
    class console;

    class game {
    public:
        game();
        ~game();

        SDL_Window *win() { return m_win; }
        SDL_GLContext glctx() { return m_glctx; }

        void run();
        void stop() { m_running = false; }
        bool is_running() const { return m_running; }

        void handle_event(SDL_Event &evt);
        void update(pd::timedelta_t dt);
        void draw() const;
        void swap();

        pd::screen *screen() { return m_screen; }
        void screen(pd::screen *val) { m_screen = val; }
        pd::console &console() { return *m_console; }
        static pd::game &instance() { return *s_instance; }

        void get_size(int &width, int &height) const { return SDL_GetWindowSize(m_win, &width, &height); }
        int width() const { int width, height; get_size(width, height); return width; }
        int height() const { int width, height; get_size(width, height); return height; }
        void fullscreen(bool val);
        bool fullscreen() const { return m_fullscreen; }
        void toggle_fullscreen() { fullscreen(!fullscreen()); }

        pd::resource_manager &resmgr() { return m_resmgr; }

    private:
        void on_resize();

        SDL_Window *m_win;
        SDL_GLContext m_glctx;
        pd::screen *m_screen;
        bool m_running;
        pd::console *m_console;
        pd::resource_manager m_resmgr;
        static pd::game *s_instance;
        uint64_t m_last_delay;
        bool m_fullscreen;

#if PD_PLATFORM == PD_PLATFORM_OSX
        int m_osx_alternative_width;
        int m_osx_alternative_height;
#endif
    };


    /* shortcut for the lazy */
    template <class T>
    T *get_resource(const std::string &filename)
    {
        return game::instance().resmgr().get<T>(filename);
    }
}

#endif
