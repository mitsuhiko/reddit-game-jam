#ifndef _INC_PD_FONT_HPP_
#define _INC_PD_FONT_HPP_
#include <pd/pd.hpp>
#include <map>

namespace pd {

    class texture;

    struct glyph_info {
        glyph_info(int advance, int xoff, int yoff,
                   const pd::texture *texture)
        {
            this->advance = advance;
            this->xoff = xoff;
            this->yoff = yoff;
            this->texture = texture;
        }

        int advance;
        int xoff;
        int yoff;
        const pd::texture *texture;
    };

    class font {
    public:
        virtual ~font() {}

        virtual const glyph_info &get(unsigned char charpoint) const = 0;
        virtual const pd::texture *texture() const = 0;
        virtual int height() const = 0;
    };

    class bitmap_font : public font {
    public:
        bitmap_font(const std::string &filename);
        ~bitmap_font();
        const glyph_info &get(unsigned char charpoint) const;
        const pd::texture *texture() const { return m_texture; }
        int height() const { return m_height; }

    private:
        std::map<unsigned char, glyph_info> m_glyphs;
        glyph_info m_dummy_char;
        int m_height;
        pd::texture *m_texture;
    };
}

#endif
