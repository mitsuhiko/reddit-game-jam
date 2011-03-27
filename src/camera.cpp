#include <pd/camera.hpp>
#include <pd/game.hpp>
#include <pd/drawtools.hpp>


pd::camera::camera()
{
    int width, height;
    pd::game::instance().get_size(width, height);
    m_x = width / 2.0f;
    m_y = height / 2.0f;
}

void pd::camera::look_at(float x, float y, float dt)
{
    m_x += (x - m_x) * dt;
    m_y += (y - m_y) * dt;
}

void pd::camera::apply()
{
    int width, height;
    pd::game::instance().get_size(width, height);
    float center_x = width / 2.0f;
    float center_y = height / 2.0f;
    pd::translate(-(m_x - center_x), -(m_y - center_y));
}