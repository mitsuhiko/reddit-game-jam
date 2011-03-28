#include <pd/player.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/entity.hpp>
#include <vector>


static const float jump_speed = 300.0f;


pd::player::player(pd::game_session *session, float x, float y)
    : pd::entity(session, x, y, 60.0f, 90.0f),
      m_thermal_idle_anim(pd::get_resource<pd::texture>("textures/character_thermal_idle.png"), 17, 0.05f),
      m_flamethrower_anim(pd::get_resource<pd::texture>("textures/flamer.png"), 5, 0.1f)
{
    stance(thermal_stance);
    hovering(12.0f);
    health(200.0f);

    m_energy = 1.0f;
    m_shooting = false;
}

void pd::player::take_damage(float val, damage_type type)
{
    switch (type) {
    case pd::entity::kinetic_damage:
        if (stance() == pd::entity::kinetic_stance) {
            m_energy = std::min(1.0f, m_energy + val / 150.0f);
            return;
        }
        break;
    case pd::entity::electromagnetic_damage:
        if (stance() == pd::entity::electromagnetic_stance) {
            m_energy = std::min(1.0f, m_energy + val / 300.0f);
            return;
        }
        break;
    case pd::entity::thermal_damage:
        if (stance() == pd::entity::thermal_stance) {
            m_energy = std::min(1.0f, m_energy + val / 100.0f);
            return;
        }
        break;
    }

    /* na, just damage like a human being */
    pd::entity::take_damage(val, type);
}
        
void pd::player::move_left(pd::timedelta_t dt)
{
    flipped(true);
    if (!collides_left())
        move(-dt * movement_speed, 0.0f);
}

void pd::player::move_right(pd::timedelta_t dt)
{
    flipped(false);
    if (!collides_right())
        move(dt * movement_speed, 0.0f);
}

void pd::player::jump()
{
    if (!airborne())
        velocity(velocity() - glm::vec2(0.0f, jump_speed));
}

void pd::player::update(pd::timedelta_t dt)
{
    m_thermal_idle_anim.update(dt);
    m_flamethrower_anim.update(dt);

    move(velocity() * dt);
    apply_gravity(dt);
}

void pd::player::local_render(pd::timedelta_t dt) const
{    
    m_thermal_idle_anim.render();
    if (m_shooting)
        m_flamethrower_anim.render(70.0f, 14.0f);
}