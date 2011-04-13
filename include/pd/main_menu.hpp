#ifndef _INC_PD_MAIN_MENU_HPP_
#define _INC_PD_MAIN_MENU_HPP_
#include <pd/pd.hpp>
#include <pd/screen.hpp>

namespace pd {

    class texture;
    class font;

    class main_menu : public screen {
    public:
        void update(pd::timedelta_t dt);
        void handle_event(SDL_Event &evt);
        void draw() const;

        static main_menu *instance();

    private:
        main_menu();

        static main_menu *s_instance;
        pd::texture *m_logo;
        pd::font *m_font;
        int m_active_item;
    };
}

#endif
