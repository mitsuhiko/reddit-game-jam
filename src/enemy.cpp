#include <pd/enemy.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>


pd::enemy::enemy(pd::game_session *session, const pd::vec2 &pos)
    : pd::entity(session, pos)
{
}
