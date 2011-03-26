#ifndef _INC_PD_MAIN_GAME_SESSION_HPP_
#define _INC_PD_MAIN_GAME_SESSION_HPP_
#include <pd/pd.hpp>
#include <pd/screen.hpp>
#include <pd/map.hpp>

namespace pd {

    class texture;
    class font;

    class game_session : public screen {
    public:
		pd::map *m_map;

        game_session();
        ~game_session();

        void update(float dt);
        void handle_event(SDL_Event &evt, float dt);
        void render(float dt) const;
    };
}

#endif
