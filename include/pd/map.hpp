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

    class animation;
    class block;
    class texture;
    class game_session;

    class map {
    public:
        typedef unsigned char tile_id_t;

	    map(pd::game_session *session, std::string filename);
	    ~map();
        
        void update(pd::timedelta_t dt);
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

        void draw_tile(int x, int y, tile_id_t tile) const;

        void get_corner_tiles(const pd::aabb &bb, int *left_tile,
                              int *right_tile, int *top_tile,
                              int *bottom_tile) const;
        tile_collision_flag get_collision(int x, int y) const;

        pd::game_session *session() { return m_session; }
        const pd::game_session *session() const { return m_session; }

    private:
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
        block(pd::map *map, pd::map::tile_id_t tile, int x, int y);
        pd::aabb bounding_box() const;
        int x() const { return m_x; }
        int y() const { return m_y; }
        const pd::map::tile_id_t tile() const { return m_tile; }
        tile_collision_flag collision() const;

        static pd::map::tile_id_t tile_by_name(const std::string &name);
        bool is(const std::string &tile) const { return is(tile_by_name(tile)); }
        bool is(pd::map::tile_id_t tile) const { return m_tile == tile; }

        void start_transition(const std::string &tile)
        {
            start_transition(tile_by_name(tile));
        }

        void start_transition(pd::map::tile_id_t new_tile);

        void update(pd::timedelta_t dt);
        void draw() const;

    private:
        pd::map *m_map;
        pd::map::tile_id_t m_tile;
        pd::animation *m_transition;
        int m_x;
        int m_y;
    };
}
#endif
