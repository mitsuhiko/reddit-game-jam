#ifndef _INC_PD_ANIMATION_HPP_
#define _INC_PD_ANIMATION_HPP_
#include <pd/pd.hpp>
#include <pd/drawtools.hpp>

namespace pd {

    class texture;
    namespace config { struct animation_config; }

    class animation {
    public:
        animation(const pd::config::animation_config &cfg);
        animation(pd::texture *texture, int frames, float anim_speed = 0.0f);
        bool update(pd::timedelta_t dt);

        void draw(const pd::vec2 &pos = pd::vec2(),
                  draw_effect effect = draw_without_effect,
                  pd::color color = pd::color()) const;
        void draw_frame(int frame, const pd::vec2 &pos = pd::vec2(),
                        draw_effect effect = draw_without_effect,
                        pd::color color = pd::color()) const;

        int width() const;
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
