#ifndef _INC_PD_DRAWTOOLS_HPP_
#define _INC_PD_DRAWTOOLS_HPP_
#include <pd/pd.hpp>
#include <pd/color.hpp>

namespace pd {

    class texture;
    class font;

    void clear_screen(pd::color color);
    void reset_color();
    void set_color(pd::color color);
    void draw_textured_quad(float x, float y, float width, float height, const pd::texture *texture);
    void draw_textured_quad(float x, float y, const pd::texture *texture);
    void draw_text(const std::string &text, float x, float y, const pd::font *font);

    /* alternative names for opengl methods and some other thin
       wrappers around them.  Why? Consistency and a chance to
       move to custom matrix stacks. */
    inline void push_matrix() { glPushMatrix(); }
    inline void pop_matrix() { glPopMatrix(); }
    inline void translate(float x, float y) { glTranslatef(x, y, 0.0f); }

    inline void rotate(float angle)
    {
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
    }

    inline void rotate_around_point(float angle, float x, float y)
    {
        translate(x, y);
        rotate(angle);
        translate(-x, -y);
    }
}

#endif
