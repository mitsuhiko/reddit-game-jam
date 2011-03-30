#ifndef _INC_PD_DRAWTOOLS_HPP_
#define _INC_PD_DRAWTOOLS_HPP_
#include <pd/pd.hpp>
#include <pd/color.hpp>

namespace pd {

        class texture;
        class font;
        class aabb;

        enum draw_effect {
            draw_without_effect = 0,
            draw_flipped_horizontally = 1,
            draw_flipped_vertically = 2
        };

        void clear_screen(pd::color color);

        void draw_quad(const pd::texture *texture);
        void draw_quad(const pd::texture *texture, const glm::vec2 &pos,
                       draw_effect effect = draw_without_effect,
                       pd::color color = pd::color());
        void draw_quad(const pd::texture *texture, const glm::vec2 &pos,
                       float width, float height,
                       draw_effect effect = draw_without_effect,
                       pd::color color = pd::color());

        void draw_debug_box(const glm::vec2 &pos, float width, float height,
                            pd::color color = pd::color());
        void draw_debug_box(const pd::aabb &aabb,
                            pd::color color = pd::color());

        void draw_text(const std::string &text, const glm::vec2 &pos,
                       const pd::font *font, pd::color color = pd::color());

        /* alternative names for opengl methods and some other thin
           wrappers around them.  Why? Consistency and a chance to
           move to custom matrix stacks. */
        inline void push_matrix() { glPushMatrix(); }
        inline void pop_matrix() { glPopMatrix(); }
        inline void translate(const glm::vec2 &pos)
        {
            glTranslatef(pos.x, pos.y, 0.0f);
        }

        inline void rotate(float angle)
        {
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
        }

        inline void rotate_around_point(float angle, const glm::vec2 &pos)
        {
            if (angle >= -10e-5 && angle <= 10e-5f)
                return;
            translate(pos);
        rotate(angle);
        translate(-pos);
    }

    inline void scale(const glm::vec2 &factor)
    {
        glScalef(factor.x, factor.y, 1.0f);
    }
}

#endif
