#include <pd/animation.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>


pd::animation::animation(pd::texture *texture, int frames, float anim_speed)
{
    m_texture = texture;
    m_frames = frames;
    m_anim_speed = anim_speed;
    m_time = 0.0f;
    m_current_frame = 0;
}

void pd::animation::update(pd::timedelta_t dt)
{
    for (m_time += dt; m_time >= m_anim_speed; m_time -= m_anim_speed)
        m_current_frame = (m_current_frame + 1) % m_frames;
}

void pd::animation::render(const glm::vec2 &pos) const
{
    render_frame(m_current_frame, pos);
}

void pd::animation::render_frame(int frame, const glm::vec2 &pos) const
{
    assert(frame < m_frames);
    int slice_width = m_texture->width() / m_frames;
    pd::texture_slice texture(m_texture, frame * slice_width,
        0, slice_width, m_texture->height());
    pd::draw_textured_quad(pos, &texture);
}