#include <pd/camera.hpp>
#include <pd/game.hpp>
#include <pd/drawtools.hpp>
#include <pd/math.hpp>
#include <pd/config.hpp>

static pd::config::_camera &cfg = pd::config::camera;


pd::camera::camera()
{
    int width, height;
    pd::game::instance().get_size(width, height);
    m_pos = pd::vec2(width / 2.0f, height / 2.0f);
}

void pd::camera::look_at(const pd::vec2 &pos, pd::timedelta_t dt)
{
    float t = cfg.adjust_time;
    pd::vec2 delta = pos - m_pos;
    m_acceleration = (2.0f * (delta - m_velocity * t)) / (t * t);
    m_velocity += m_acceleration * dt;
    m_pos += m_velocity * dt;
}

void pd::camera::apply()
{
    int width, height;
    pd::game::instance().get_size(width, height);

    // XXX: quick hack to avoid texture filtering problems when camera
    // moves in other units than a pixel.
    pd::vec2 delta = -m_pos + pd::vec2(width / 2.0f, height / 2.0f);
    pd::translate(pd::floor(delta.x), pd::floor(delta.y));
}
