#ifndef _INC_PD_ANIMATION_HPP_
#define _INC_PD_ANIMATION_HPP_
#include <pd/pd.hpp>

namespace pd {

    class texture;

    class animation {
    public:
        animation(pd::texture *texture, int frames, float anim_speed = 0.0f);
        void update(pd::timedelta_t dt);
        void render(float x = 0.0f, float y = 0.0f) const;
        void render_frame(int frame, float x = 0.0f, float y = 0.0f) const;

        int current_frame() const { return m_current_frame; }
        int frames() const { return m_frames; }
        float anim_speed() const { return m_anim_speed; }
        void anim_speed(float val) { m_anim_speed = val; }
        pd::texture *texture() { return m_texture; }
        const pd::texture *texture() const { return m_texture; }

    private:
        pd::texture *m_texture;
        int m_frames;
        int m_current_frame;
        float m_anim_speed;
        float m_time;
    };

}

#endif
