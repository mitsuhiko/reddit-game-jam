#include <pd/player.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>

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
    if (vec.x < -4.0f)
        return;
    body()->ApplyForce(b2Vec2(-1000.0f, 0.0f), body()->GetWorldCenter());
}

void pd::player::move_right()
{
    flipped(false);
    const b2Vec2 &vec = body()->GetLinearVelocity();
    if (vec.x > 4.0f)
        return;
    body()->ApplyForce(b2Vec2(1000.0f, 0.0f), body()->GetWorldCenter());
}

void pd::player::jump()
{
    if (!on_ground())
        return;
    body()->ApplyLinearImpulse(b2Vec2(0.0f, -400.0f), body()->GetWorldCenter());
}

void pd::player::update(float dt)
{
    m_walk_anim.update(dt);
}

void pd::player::local_render(float dt) const
{
    m_walk_anim.render(dt);
}