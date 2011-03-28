#ifndef _INC_PD_ENEMY_HPP_
#define _INC_PD_ENEMY_HPP_
#include <pd/pd.hpp>
#include <pd/entity.hpp>
#include <pd/player.hpp>

namespace pd {

    class session;

    class enemy : public pd::entity {
    public:
        enemy(pd::game_session *session, const glm::vec2 &pos,
              float width, float height);
    };
}

#endif
