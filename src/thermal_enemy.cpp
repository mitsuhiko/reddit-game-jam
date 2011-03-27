#include <pd/thermal_enemy.hpp>
#include <pd/game.hpp>
#include <pd/texture.hpp>


pd::thermal_enemy::thermal_enemy(pd::game_session *session, float x, float y)
    : pd::enemy(session, x, y, 36.0f, 40.0f, 0.0f, thermal_stance),
    m_walk_anim(pd::get_resource<pd::texture>("textures/test-enemy.png"), 4, 0.2f)
{
}

void pd::thermal_enemy::update(float dt)
{
    m_walk_anim.update(dt);

    // Quick and dirty AI.
    if (!airborne()) {
        apply_impulse(0.0f, -220.0f);
    }
    b2Vec2 force = player()->position() - position();
    force *= 80.0f;
    body()->ApplyForce(force, body()->GetWorldCenter());
}

void pd::thermal_enemy::local_render(float dt) const
{
    m_walk_anim.render(dt);
}