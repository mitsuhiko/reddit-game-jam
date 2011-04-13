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
    class enemy;
    class player;
    class camera;

    /* implemented in the game_session.cpp */
    class game_power_bar;

    class game_session : public screen {
    public:

        game_session();
        ~game_session();

        void update(pd::timedelta_t dt);
        void handle_event(SDL_Event &evt);
        void draw() const;

        void draw_gui() const;
        void update_cam(pd::timedelta_t dt);

        pd::map *map() { return m_map; }
        const pd::map *map() const { return m_map; }

        pd::player *player() const { return m_player; }
        const std::vector<pd::enemy *> &enemies() const { return m_enemies; }
        std::vector<pd::enemy *> &enemies() { return m_enemies; }

    private:
        bool m_draw_bounds;
		pd::map *m_map;
        pd::player *m_player;
        std::vector<pd::enemy *> m_enemies;
        pd::camera *m_cam;

        pd::game_power_bar *m_kinetic_energy_bar;
        pd::game_power_bar *m_electromagnetic_energy_bar;
        pd::game_power_bar *m_thermal_energy_bar;
    };
}

#endif
