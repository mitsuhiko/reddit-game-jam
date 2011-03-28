#include <pd/enemy.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>


pd::enemy::enemy(pd::game_session *session, const glm::vec2 &pos,
                 float width, float height)
    : pd::entity(session, pos, width, height)
{
}