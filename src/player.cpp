#include <pd/player.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>


pd::player::player(float x, float y)
    : pd::entity(x, y),
      m_walk_anim(pd::get_resource<pd::texture>("textures/test-pirate.png"), 9, 0.1f)
{
    m_stance = kinetic_stance;
}

void pd::player::update(float dt)
{
    m_walk_anim.update(dt);
}

void pd::player::render(float dt) const
{
    m_walk_anim.render(x(), y(), dt);
}