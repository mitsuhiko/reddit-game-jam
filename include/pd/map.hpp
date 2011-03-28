#ifndef _INC_PD_MAP_HPP_
#define _INC_PD_MAP_HPP_
#include <pd/pd.hpp>
#include <pd/math.hpp>
#include <pd/color.hpp>
#include <pd/aabb.hpp>
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

	    void render() const;

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

        tile_id_t get_fg(int x, int y) const
        {
            if (x < 0 || x >= m_width || y < 0 || y >= m_height)
                return 0;
            return m_foreground[(y * m_width) + x];
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

        pd::aabb bounding_box(int x, int y) const
        {
            glm::vec2 pos(m_tile_width * x, m_tile_height * y);
            return pd::aabb(pos, pos + glm::vec2(m_tile_width, m_tile_height));
        }

        bool occupied(int x, int y) const
        {
            return get_fg(x, y) != 0 || get_block(x, y) != 0;
        }

        pd::game_session *session() { return m_session; }
        const pd::game_session *session() const { return m_session; }

    private:
        void draw_tile(int x, int y, tile_id_t tile) const;
        pd::block *try_make_block(tile_id_t tile, int x, int y);
        
	    int m_width;
	    int m_height;
	    int m_tile_width;
	    int m_tile_height;
	    tile_id_t *m_background;
	    tile_id_t *m_foreground;
        pd::block **m_blocks;
	    pd::texture *m_tileset;
        pd::game_session *m_session;
        pd::color m_background_color;
        std::map<tile_id_t, pd::texture *> m_tiles;
    };

    class block {
    public:
        enum block_type {
            metal_type,
            lava_type,
            ice_type,
            water_type,
            glass_type
        };

        block(pd::map *map, pd::texture *texture, block_type type, int x, int y);
        block_type type() const { return m_type; }
        int x() const { return m_x; }
        int y() const { return m_y; }
        const pd::texture *texture() const { return m_texture; }
        void render() const;

    private:
        pd::map *m_map;
        block_type m_type;
        pd::texture *m_texture;
        int m_x;
        int m_y;
    };
}
#endif