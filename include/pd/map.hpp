#ifndef _INC_PD_MAP_HPP_
#define _INC_PD_MAP_HPP_
#include <pd/pd.hpp>
#include <pd/math.hpp>
#include <pd/color.hpp>
#include <pd/aabb.hpp>
#include <pd/collisions.hpp>
#include <map>
#include <vector>

namespace pd {

    class block;
    class texture;
    class game_session;

    class map {
    public:
        typedef unsigned char tile_id_t;

	    map(pd::game_session *session, std::string filename);
	    ~map();
        
	    void draw() const;
        void draw_tile_bounds() const;

        int width() const { return m_width; }
        int height() const { return m_height; }
        int tile_width() const { return m_tile_width; }
        int tile_height() const { return m_tile_height; }
        pd::color background_color() const { return m_background_color; }

        tile_id_t get_bg(int x, int y) const
        {
            if (x < 0 || x >= m_width || y < 0 || y >= m_height)
                return 0;
            return m_background[(y * m_width) + x];
        }

        pd::block *get_block(int x, int y)
        {
            if (x < 0 || x >= m_width || y < 0 || y >= m_height)
                return 0;
            return m_blocks[(y * m_width) + x];
        }

        const pd::block *get_block(int x, int y) const
        {
            if (x < 0 || x >= m_width || y < 0 || y >= m_height)
                return 0;
            return m_blocks[(y * m_width) + x];
        }

        tile_collision_flag get_collision(int x, int y) const;

        pd::game_session *session() { return m_session; }
        const pd::game_session *session() const { return m_session; }

    private:
        void draw_tile(int x, int y, tile_id_t tile) const;
        
	    int m_width;
	    int m_height;
	    int m_tile_width;
	    int m_tile_height;
	    tile_id_t *m_background;
        pd::block **m_blocks;
	    pd::texture *m_tileset;
        pd::game_session *m_session;
        pd::color m_background_color;
        std::map<tile_id_t, pd::texture *> m_tiles;
    };

    class block {
    public:

        static const pd::map::tile_id_t metal_type = 129;
        static const pd::map::tile_id_t ice_type = 130;
        static const pd::map::tile_id_t glass_type = 131;
        static const pd::map::tile_id_t laval_type = 132;
        static const pd::map::tile_id_t water_type = 133;

        block(pd::map *map, pd::map::tile_id_t tile, int x, int y);
        pd::aabb bounding_box() const;
        int x() const { return m_x; }
        int y() const { return m_y; }
        const pd::map::tile_id_t tile() const { return m_tile; }
        tile_collision_flag collision() const;

    private:
        pd::map *m_map;
        pd::map::tile_id_t m_tile;
        int m_x;
        int m_y;
    };
}
#endif
