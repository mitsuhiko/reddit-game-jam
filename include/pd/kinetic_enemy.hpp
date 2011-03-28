#ifndef _INC_PD_KINETIC_ENEMY_HPP_
#define _INC_PD_KINETIC_ENEMY_HPP_
#include <pd/pd.hpp>
#include <pd/enemy.hpp>
#include <pd/animation.hpp>

namespace pd {

    class session;

    class kinetic_enemy : public pd::enemy {
    public:
        kinetic_enemy(pd::game_session *session, float x = 0.0f, float y = 0.0f);

        bool starts_dashing() const;
        void update(pd::timedelta_t dt);
        void local_render(pd::timedelta_t dt) const;

    private:
        stance_type m_stance;
        pd::animation m_walk_anim;
        pd::animation m_dash_anim;
        glm::vec2 m_velocity;
        bool m_dashing;
        float m_dash_countdown;
    };
}

#endif
