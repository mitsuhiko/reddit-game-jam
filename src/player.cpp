#include <pd/player.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>


static const float max_airborne_velocity = 3.0f;
static const float max_velocity = 6.0f;
static const float jump_impulse = 260.0f;
static const float movement_force = 1500.0f;


pd::player::player(pd::game_session *session, float x, float y)
    : pd::entity(session, x, y, 35.0f, 64.0f, -8.0f, 20.0f, 0.8f, true),
      m_walk_anim(pd::get_resource<pd::texture>("textures/test-pirate.png"), 9, 0.1f)
{
    stance(kinetic_stance);

    m_energy = 1.0f;
}

void pd::player::move_left()
{
    flipped(true);
    pd::vec2 vec = linear_velocity();
    if (vec.x > -(airborne() ? max_airborne_velocity : max_velocity))
         apply_force(-movement_force, 0.0f);
}

void pd::player::move_right()
{
    flipped(false);
    pd::vec2 vec = linear_velocity();
    if (vec.x < (airborne() ? max_airborne_velocity : max_velocity))
        apply_force(movement_force, 0.0f);
}

void pd::player::jump()
{
    if (!airborne())
        apply_impulse(0.0f, -jump_impulse);
}

void pd::player::update(float dt)
{
    m_walk_anim.update(dt);
}

void pd::player::local_render(float dt) const
{
    m_walk_anim.render(dt);
}