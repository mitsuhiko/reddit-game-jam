#include <pd/camera.hpp>
#include <pd/game.hpp>
#include <pd/drawtools.hpp>

static const float adjust_speed = 2.0f;


pd::camera::camera()
{
    int width, height;
    pd::game::instance().get_size(width, height);
    m_pos = glm::vec2(width / 2.0f, height / 2.0f);
}

void pd::camera::look_at(const glm::vec2 &pos, pd::timedelta_t dt)
{
    m_pos += (pos - m_pos) * dt * adjust_speed;
}

void pd::camera::apply()
{
    int width, height;
    pd::game::instance().get_size(width, height);
    pd::translate(-m_pos + glm::vec2(width / 2.0f, height / 2.0f));
}