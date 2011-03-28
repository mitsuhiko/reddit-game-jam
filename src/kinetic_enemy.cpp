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


pd::kinetic_enemy::kinetic_enemy(pd::game_session *session, float x, float y)
    : pd::enemy(session, x, y, 50.0f, 82.0f, thermal_stance),
    m_walk_anim(pd::get_resource<pd::texture>("textures/enemy_kinetic_walk.png"), 19, 0.035f),
    m_dash_anim(pd::get_resource<pd::texture>("textures/enemy_kinetic_dash.png"), 2)
{
    m_dashing = false;
    m_dash_countdown = -1.0f;

    velocity(glm::vec2(movement_speed, 0.0f));
}

void pd::kinetic_enemy::update(pd::timedelta_t dt)
{
    m_walk_anim.update(dt);

    if (m_dash_countdown >= 0.0f) {
        if ((m_dash_countdown -= dt) >= 0.0f)
            return;
        m_dashing = true;
        velocity(glm::vec2(dash_speed * (flipped() ? -1 : 1), velocity().y));
    } else if (!m_dashing && starts_dashing()) {
        m_dash_countdown = dash_countdown;
        velocity();
    } else if ((!flipped() && collides_right()) || (flipped() && collides_left())) {
        velocity(glm::vec2(movement_speed * (flipped() ? -1 : 1), velocity().y));
        m_dashing = false;
    }
}

bool pd::kinetic_enemy::starts_dashing() const
{
    if (airborne())
        return false;

    glm::vec2 distance = pos() - session()->player()->pos();
    if (std::abs(distance.y) > 50.0f ||
        (flipped() && distance.x < 0.0f) ||
        (!flipped() && distance.x > 0.0f))
        return false;

    return glm::length(distance) <= dash_activation_distance;
}

void pd::kinetic_enemy::local_render(pd::timedelta_t dt) const
{
    if (m_dash_countdown > 0.0f || m_dashing) {
        m_dash_anim.render_frame(m_dashing ? 1 : 0, -60.0f, 0.0f);
    } else {
        m_walk_anim.render();
    }
}