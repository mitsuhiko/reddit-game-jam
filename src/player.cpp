#include <pd/player.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>

pd::player::player(b2World *world, float x, float y)
    : pd::entity(world, x, y),
      m_walk_anim(pd::get_resource<pd::texture>("textures/test-pirate.png"), 9, 0.1f)
{
    m_stance = kinetic_stance;

    width(30.0f);
    height(50.0f);
    density(20.0f);
    friction(1.02f);
    build_box2d_object();

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