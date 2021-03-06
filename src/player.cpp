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
    : pd::entity(session, x, y, 60.0f, 90.0f, -20.0f, 20.0f, 0.0f, true),
      m_thermal_idle_anim(pd::get_resource<pd::texture>("textures/character_thermal_idle.png"), 17, 0.05f),
      m_flamethrower_anim(pd::get_resource<pd::texture>("textures/flamer.png"), 5, 0.1f)
{
    stance(thermal_stance);

    m_energy = 1.0f;
    health(200.0f);
    m_shooting = false;
    m_stoped = true;
}

void pd::player::move_left()
{
    m_stoped = false;
    m_ticks_until_stop = SDL_GetTicks();
    flipped(true);
    pd::vec2 vec = linear_velocity();
    if (vec.x > -(airborne() ? max_airborne_velocity : max_velocity))
         apply_force(-movement_force, 0.0f);
}

void pd::player::move_right()
{
    m_stoped = false;
    m_ticks_until_stop = SDL_GetTicks();
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

void pd::player::update(float dt)
{
    apply_force(0.0f, 800.0f);
    m_thermal_idle_anim.update(dt);
    m_flamethrower_anim.update(dt);

    if (m_shooting) {
        weapon_damage_test(dt);
        m_energy = std::max(0.0f, m_energy - dt * 0.35f);
        if (m_energy == 0.0f)
            m_shooting = false;
    }
}

void pd::player::weapon_damage_test(float dt)
{
    if (!m_shooting)
        return;

    if (stance() == thermal_stance) {
        std::vector<pd::entity *> entities;
        visible_entities(200.0f, entities);
        for (std::vector<pd::entity *>::iterator iter = entities.begin();
             iter != entities.end(); ++iter)
            (*iter)->take_damage(500.0f * dt, pd::entity::thermal_damage);
    }
}

void pd::player::local_render(float dt) const
{    
    m_thermal_idle_anim.render();
    if (m_shooting)
        m_flamethrower_anim.render(70.0f, 14.0f);
}

void pd::player::stop()
{
    if (((SDL_GetTicks() - m_ticks_until_stop) > 100) && !airborne() && !m_stoped) {
        body()->SetLinearVelocity(b2Vec2(0.0f, body()->GetLinearVelocity().y));
        m_stoped = true;
    }
}
