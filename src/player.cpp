#include <pd/player.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/entity.hpp>
#include <vector>


static const float max_airborne_velocity = 3.0f;
static const float max_velocity = 6.0f;
static const float jump_impulse = 1250.0f;
static const float movement_force = 4000.0f;

pd::player::player(pd::game_session *session, float x, float y)
    : pd::entity(session, x, y, 60.0f, 90.0f),
      m_thermal_idle_anim(pd::get_resource<pd::texture>("textures/character_thermal_idle.png"), 17, 0.05f),
      m_flamethrower_anim(pd::get_resource<pd::texture>("textures/flamer.png"), 5, 0.1f)
{
    stance(thermal_stance);

    m_energy = 1.0f;
    m_shooting = false;
    health(200.0f);
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
        
void pd::player::move_left(float dt)
{
}

void pd::player::move_right(float dt)
{
}

void pd::player::jump()
{
}

void pd::player::update(float dt)
{
    m_thermal_idle_anim.update(dt);
    m_flamethrower_anim.update(dt);
}

void pd::player::local_render(float dt) const
{    
    m_thermal_idle_anim.render();
    if (m_shooting)
        m_flamethrower_anim.render(70.0f, 14.0f);
}