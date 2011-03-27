#ifndef _INC_PD_PLAYER_HPP_
#define _INC_PD_PLAYER_HPP_
#include <pd/pd.hpp>
#include <pd/entity.hpp>
#include <pd/animation.hpp>

namespace pd {

    class session;

    class player : public pd::entity {
    public:
        player(pd::game_session *session, float x = 0.0f, float y = 0.0f);
        float energy() const { return m_energy; }
        void move_left();
        void move_right();
        void jump();
        void stop();

        void update(float dt);
        void local_render(float dt) const;

    private:
        pd::animation m_thermal_idle_anim;
        float m_energy;
        int m_ticks_until_stop;
    };
}

#endif
