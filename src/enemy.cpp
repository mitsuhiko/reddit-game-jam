#include <pd/enemy.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>


pd::enemy::enemy(pd::game_session *session, float x, float y,
                 float width, float height, stance_type stance)
    : pd::entity(session, x, y, width, height)
{
    this->stance(stance);
}