#ifndef _INC_PD_TEXTURE_HPP_
#define _INC_PD_TEXTURE_HPP_
#include <pd/pd.hpp>
#include <pd/resource_base.hpp>

namespace pd {

    class texture;

    texture *texture_from_surface(SDL_Surface *surface);
    texture *load_texture(const std::string &filename);

    class texture : public pd::resource_base {
    public:
        virtual ~texture() {}

        virtual GLuint id() const = 0;
        virtual int width() const = 0;
        virtual int height() const = 0;
        virtual int stored_width() const = 0;
        virtual int stored_height() const = 0;
        virtual int offset_x() const = 0;
        virtual int offset_y() const = 0;
        virtual const texture *parent() const = 0;

        texture *slice(int x, int y, int width, int height);

        static texture *load_as_resource(const std::string &filename)
        {
            return load_texture(filename);
        }
    };

    class simple_texture : public texture {
    public:
        simple_texture(SDL_Surface *surface);
        ~simple_texture();

        GLuint id() const { return m_id; }
        int width() const { return m_width; }
        int height() const { return m_height; }
        int stored_width() const { return m_stored_width; }
        int stored_height() const { return m_stored_height; }
        int offset_x() const { return 0; }
        int offset_y() const { return 0; }
        const texture *parent() const { return 0; }

    private:
        GLuint m_id;
        int m_width;
        int m_height;
        int m_stored_width;
        int m_stored_height;
    };

    class texture_slice : public texture {
    public:
        texture_slice(texture *parent, int x, int y, int width, int height);
        GLuint id() const { return m_parent->id(); }
        int width() const { return m_width; }
        int height() const { return m_height; }
        int stored_width() const { return m_parent->stored_width(); }
        int stored_height() const { return m_parent->stored_height(); }
        int offset_x() const { return m_offset_x; }
        int offset_y() const { return m_offset_y; }
        const texture *parent() const { return m_parent; }

        texture *slice(int x, int y, int width, int height);

    private:
        texture *m_parent;
        int m_offset_x;
        int m_offset_y;
        int m_width;
        int m_height;
    };
}

#endif
