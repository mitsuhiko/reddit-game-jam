#ifndef _INC_PD_MAIN_GAME_SESSION_HPP_
#define _INC_PD_MAIN_GAME_SESSION_HPP_
#include <pd/pd.hpp>
#include <pd/screen.hpp>
#include <pd/map.hpp>
#include <vector>

namespace pd {

    class texture;
    class font;
    class entity;

    class game_session : public screen {
    public:

        game_session();
        ~game_session();

        void add_entity(pd::entity *entity);
        bool remove_entity(pd::entity *entity);

        void update(float dt);
        void handle_event(SDL_Event &evt, float dt);
        void render(float dt) const;

    private:
		pd::map *m_map;
        pd::entity *m_player;
        std::vector<pd::entity *> m_entities;
    };
}

#endif
