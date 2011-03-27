#include <pd/player.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>


static const float max_airborne_velocity = 3.0f;
static const float max_velocity = 6.0f;
static const float jump_impulse = 240.0f;
static const float movement_force = 1500.0f;


pd::player::player(pd::game_session *session, float x, float y)
    : pd::entity(session, x, y, 30.0f, 60.0f, 20.0f, 0.8f, true),
      m_walk_anim(pd::get_resource<pd::texture>("textures/test-pirate.png"), 9, 0.1f)
{
    m_stance = kinetic_stance;

    m_thermal_energy = 1.0f;
    m_electromagnetic_energy = 1.0f;
    m_kinetic_energy = 1.0f;
}

void pd::player::move_left()
{
    flipped(true);
    const b2Vec2 &vec = body()->GetLinearVelocity();
    if (vec.x < -(on_ground() ? max_velocity : max_airborne_velocity))
        return;
    body()->ApplyForce(b2Vec2(-movement_force, 0.0f), body()->GetWorldCenter());
}

void pd::player::move_right()
{
    flipped(false);
    const b2Vec2 &vec = body()->GetLinearVelocity();
    if (vec.x > (on_ground() ? max_velocity : max_airborne_velocity))
        return;
    body()->ApplyForce(b2Vec2(movement_force, 0.0f), body()->GetWorldCenter());
}

void pd::player::jump()
{
    if (!on_ground())
        return;
    body()->ApplyLinearImpulse(b2Vec2(0.0f, -jump_impulse), body()->GetWorldCenter());
}

void pd::player::update(float dt)
{
    m_walk_anim.update(dt);
}

void pd::player::local_render(float dt) const
{
    m_walk_anim.render(dt);
}