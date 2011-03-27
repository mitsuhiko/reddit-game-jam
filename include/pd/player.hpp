#ifndef _INC_PD_PLAYER_HPP_
#define _INC_PD_PLAYER_HPP_
#include <pd/pd.hpp>
#include <pd/entity.hpp>
#include <pd/animation.hpp>
#include <Box2D/Box2D.h>

namespace pd {

    class texture;
    class session;

    class player : public pd::entity {
    public:
        enum stance_type {
            thermal_stance,
            electromagnetic_stance,
            kinetic_stance
        };

        player(pd::game_session *session, float x = 0.0f, float y = 0.0f);
        stance_type stance() const { return m_stance; }
        void stance(stance_type val) { m_stance = val; }
        float energy() const { return m_energy; }
        void move_left();
        void move_right();
        void jump();

        void update(float dt);
        void local_render(float dt) const;

    private:
        stance_type m_stance;
        pd::animation m_walk_anim;
        float m_energy;
    };
}

#endif
