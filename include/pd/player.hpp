#ifndef _INC_PD_PLAYER_HPP_
#define _INC_PD_PLAYER_HPP_
#include <pd/pd.hpp>
#include <pd/entity.hpp>
#include <pd/animation.hpp>

namespace pd {

    class texture;

    class player : public pd::entity {
    public:
        enum stance_type {
            thermal_stance,
            electromagnetic_stance,
            kinetic_stance
        };

        player(float x = 0.0f, float y = 0.0f);
        stance_type stance() const { return m_stance; }
        void stance(stance_type val) { m_stance = val; }

        void update(float dt);
        void render(float dt) const;

    private:
        stance_type m_stance;
        pd::animation m_walk_anim;
    };
}

#endif
