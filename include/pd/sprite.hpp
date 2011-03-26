#ifndef _INC_PD_SPRITE_HPP_
#define _INC_PD_SPRITE_HPP_
#include <pd/pd.hpp>

namespace pd {

    class texture;

    class sprite {
    public:
        sprite(const pd::texture *texture, float x = 0.0f, float y = 0.0f);

        float x() const { return m_x; }
        void x(float val) { m_x = val; }
        float y() const { return m_y; }
        void y(float val) { m_y = val; }
        void move(float x, float y);
        void render(float dt) const;

    private:
        const pd::texture *m_texture;
        float m_x;
        float m_y;
    };
}

#endif
