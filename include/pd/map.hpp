#ifndef _INC_PD_MAP_HPP_
#define _INC_PD_MAP_HPP_
#include <pd/pd.hpp>
#include <pd/math.hpp>
#include <pd/color.hpp>
#include <map>

namespace pd {

    class texture;
    class game_session;

    class map {
    public:
        typedef unsigned char tile_id_t;

	    map(pd::game_session *session, std::string filename);
	    ~map();

	    void render() const;

        int width() const { return m_width; }
        int height() const { return m_height; }
        int tile_width() const { return m_tile_width; }
        int tile_height() const { return m_tile_height; }
        pd::color background_color() const { return m_background_color; }

        tile_id_t get_bg(int x, int y) const { return m_background[(y * m_width) + x]; }
        tile_id_t get_fg(int x, int y) const { return m_foreground[(y * m_width) + x]; }

    private:
        void draw_tile(int x, int y, tile_id_t tile) const;
        void create_ground_box(int x, int y, int length);
        
	    int m_width;
	    int m_height;
	    int m_tile_width;
	    int m_tile_height;
	    tile_id_t *m_background;
	    tile_id_t *m_foreground;
	    pd::texture *m_tileset;
        pd::game_session *m_session;
        pd::color m_background_color;
        std::map<tile_id_t, pd::texture *> m_tiles;
    };
}
#endif