#include <pd/player.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>

pd::player::player(pd::game_session *session, float x, float y)
    : pd::entity(session, x, y, 30.0f, 60.0f, 20.0f, 2.32f, true),
      m_walk_anim(pd::get_resource<pd::texture>("textures/test-pirate.png"), 9, 0.1f)
{
    m_stance = kinetic_stance;

    m_thermal_energy = 1.0f;
    m_electromagnetic_energy = 1.0f;
    m_kinetic_energy = 1.0f;
}

void pd::player::update(float dt)
{
    m_walk_anim.update(dt);
}

void pd::player::render(float dt) const
{
    m_walk_anim.render(x(), y(), dt);
}