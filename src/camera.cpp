#include <pd/camera.hpp>
#include <pd/game.hpp>
#include <pd/drawtools.hpp>
#include <pd/math.hpp>

static const float adjust_time = 0.35f;


pd::camera::camera()
{
    int width, height;
    pd::game::instance().get_size(width, height);
    m_pos = glm::vec2(width / 2.0f, height / 2.0f);
}

void pd::camera::look_at(const glm::vec2 &pos, pd::timedelta_t dt)
{
    float t = adjust_time;
    glm::vec2 delta = pos - m_pos;
    m_acceleration = (2.0f * (delta - m_velocity * t)) / (t * t);
    m_velocity += m_acceleration * dt;
    m_pos += m_velocity * dt;
}

void pd::camera::apply()
{
    int width, height;
    pd::game::instance().get_size(width, height);
    pd::translate(-m_pos + glm::vec2(width / 2.0f, height / 2.0f));
}
