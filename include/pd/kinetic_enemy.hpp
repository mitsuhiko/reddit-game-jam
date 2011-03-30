#ifndef _INC_PD_KINETIC_ENEMY_HPP_
#define _INC_PD_KINETIC_ENEMY_HPP_
#include <pd/pd.hpp>
#include <pd/enemy.hpp>
#include <pd/animation.hpp>

namespace pd {

    class session;

    class kinetic_enemy : public pd::enemy {
    public:
        kinetic_enemy(pd::game_session *session, const glm::vec2 &pos);

        bool starts_dashing() const;
        void update(pd::timedelta_t dt);
        void render(pd::timedelta_t dt) const;

    private:
        pd::animation m_walk_anim;
        pd::animation m_dash_anim;
        int m_direction;
        bool m_dashing;
        pd::timedelta_t m_dash_countdown;
    };
}

#endif
