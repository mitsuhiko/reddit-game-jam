#ifndef _INC_PD_KINETIC_ENEMY_HPP_
#define _INC_PD_KINETIC_ENEMY_HPP_
#include <pd/pd.hpp>
#include <pd/enemy.hpp>
#include <pd/animation.hpp>

namespace pd {

    class session;

    class kinetic_enemy : public pd::enemy {
    public:
        enum state {
            spawn_state,
            walking_state,
            prepare_dashing_state,
            dashing_state,
            dash_recover_state
        };

        kinetic_enemy(pd::game_session *session, const pd::vec2 &pos);

        bool can_see(const pd::entity *other) const;
        bool dashing() const { return m_state == dashing_state; }

        float width() const;
        float height() const;

        void update(pd::timedelta_t dt);
        void render(pd::timedelta_t dt) const;

    private:
        pd::animation m_walk_anim;
        pd::animation m_dash_anim;
        int m_direction;
        bool m_hit_player;
        state m_state;
        pd::timedelta_t m_state_countdown;
    };
}

#endif
