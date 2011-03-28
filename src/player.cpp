#include <pd/player.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/entity.hpp>
#include <vector>


static const float jump_speed = 300.0f;
static const float movement_speed = 240.0f;
static const float acceleration_factor = 2.0f;


pd::player::player(pd::game_session *session, const glm::vec2 &pos)
    : pd::entity(session, pos, 60.0f, 90.0f),
      m_thermal_idle_anim(pd::get_resource<pd::texture>("textures/character_thermal_idle.png"), 17, 0.05f),
      m_flamethrower_anim(pd::get_resource<pd::texture>("textures/flamer.png"), 5, 0.1f)
{
    m_energy = 1.0f;
    m_movement_time = 0.0f;
}

void pd::player::update(pd::timedelta_t dt)
{
    m_thermal_idle_anim.update(dt);
    m_flamethrower_anim.update(dt);
}

void pd::player::render(pd::timedelta_t dt) const
{
    static const glm::vec2 hovering(0.0f, -12.0f);
    m_thermal_idle_anim.render(pos() + hovering);
    m_flamethrower_anim.render(pos() + glm::vec2(70.0f, 14.0f) + hovering);
}