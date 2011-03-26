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
    class player;

    /* implemented in the game_session.cpp */
    class game_power_bar;

    class game_session : public screen {
    public:

        game_session();
        ~game_session();

        void add_entity(pd::entity *entity);
        bool remove_entity(pd::entity *entity);

        void update(float dt);
        void handle_event(SDL_Event &evt, float dt);
        void render(float dt) const;

        void render_gui(float dt) const;

    private:
		pd::map *m_map;
        pd::player *m_player;
        std::vector<pd::entity *> m_entities;

        pd::game_power_bar *m_kinetic_energy_bar;
        pd::game_power_bar *m_electromagnetic_energy_bar;
        pd::game_power_bar *m_thermal_energy_bar;
        pd::game_power_bar *m_small_kinetic_energy_bar;
        pd::game_power_bar *m_small_electromagnetic_energy_bar;
        pd::game_power_bar *m_small_thermal_energy_bar;
    };
}

#endif
