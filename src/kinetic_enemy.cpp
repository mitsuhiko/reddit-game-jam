#include <pd/kinetic_enemy.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game_session.hpp>

static const float movement_speed = 100.0f;
static const float dash_speed = 700.0f;
static const float dash_countdown = 0.3f;
static const float dash_activation_distance = 300.0f;


pd::kinetic_enemy::kinetic_enemy(pd::game_session *session,
                                 const glm::vec2 &pos)
    : pd::enemy(session, pos, 50.0f, 82.0f),
    m_walk_anim(pd::get_resource<pd::texture>("textures/enemy_kinetic_walk.png"), 19, 0.035f),
    m_dash_anim(pd::get_resource<pd::texture>("textures/enemy_kinetic_dash.png"), 2)
{
    m_dashing = false;
    m_dash_countdown = -1.0f;
}

void pd::kinetic_enemy::update(pd::timedelta_t dt)
{
    m_walk_anim.update(dt);
}

bool pd::kinetic_enemy::starts_dashing() const
{
    return false;
}

void pd::kinetic_enemy::render(pd::timedelta_t dt) const
{
    if (m_dash_countdown > 0.0f || m_dashing) {
        m_dash_anim.render_frame(m_dashing ? 1 : 0, pos() - glm::vec2(60.0f, 0.0f));
    } else {
        m_walk_anim.render(pos());
    }
}