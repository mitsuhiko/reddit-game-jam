#ifndef _INC_PD_MAP_HPP_
#define _INC_PD_MAP_HPP_

#include <pd/pd.hpp>
#include <pd/texture.hpp>
#include <string>

namespace pd {

    class map {
    public:
        typedef unsigned char tile_id_t;

	    map(std::string filename);
	    bool load(std::string filename);
	    void render();
	    ~map();

        int width() const { return m_width; }
        int height() const { return m_height; }
        int tile_width() const { return m_tile_width; }
        int tile_height() const { return m_tile_height; }

        tile_id_t get_bg(int x, int y) const { return m_background[x][y]; }
        tile_id_t get_fg(int x, int y) const { return m_foreground[x][y]; }

    private:
	    int m_width;
	    int m_height;
	    int m_tile_width;
	    int m_tile_height;
	    tile_id_t **m_background;
	    tile_id_t **m_foreground;
	    pd::texture *m_text;
    };
}
#endif