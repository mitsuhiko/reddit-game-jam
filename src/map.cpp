#include <pd/map.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/animation.hpp>
#include <pd/lexical_cast.hpp>
#include <pd/xml.hpp>
#include <pd/math.hpp>
#include <sstream>


static void read_block_data(pd::map::tile_id_t *target, int width, int height,
                            const std::string &csv)
{
    std::stringstream ss(csv);

    for (int y = 0; y < height; y++) {
        std::string line;
        std::getline(ss, line);
        std::stringstream liness(line);
        for (int x = 0; x < width; x++) {
            std::string val;
            std::getline(liness, val, ',');
            int t = pd::lexical_cast<int>(val);
            target[(y * width) + x] = (pd::map::tile_id_t)t;
        }
    }
}


pd::map::map(pd::game_session *session, std::string filename)
{
    m_session = session;

    pd::xml_document doc;
    if (!doc.load(filename)) {
        std::stringstream ss;
        ss << "Unable to load map from '" << filename << "'";
        pd::critical_error("Cannot load map", ss.str());
    }

    pd::xml_element root = doc.root();

    // general properties
    m_width = pd::lexical_cast<int>(root.attr("width"));
    m_height = pd::lexical_cast<int>(root.attr("height"));
    m_tile_width = pd::lexical_cast<int>(root.attr("tilewidth"));
    m_tile_height = pd::lexical_cast<int>(root.attr("tileheight"));
    pd::xml_element properties = root.first_child("properties");
    for (pd::xml_element prop = properties.first_child(); prop;
         prop = prop.next_sibling()) {
        if (prop.attr("name") == "background") {
            m_background_color = pd::color(prop.attr("value"));
            break;
        }
    }

    m_background = 0;
    tile_id_t *foreground = 0;

    for (pd::xml_element layer = root.first_child("layer"); layer;
         layer = layer.next_sibling("layer")) {
        pd::xml_element data = layer.first_child();
        if (data.attr("encoding") != "csv")
            pd::critical_error("Cannot load map",
                "Currently only CSV encoded TMX files are supported");

        std::string name = layer.attr("name");
        tile_id_t **target;
        if (name == "background")
            target = &m_background;
        else if (name == "foreground")
            target = &foreground;
        else
            pd::critical_error("Cannot load map",
                "Only two layers allowed 'foreground' and 'background'");
        if (*target)
            pd::critical_error("Cannot load map", "Map layer defined twice");
        *target = new pd::map::tile_id_t[m_width * m_height];
        read_block_data(*target, m_width, m_height, data.text());
    }

    if (!m_background || !foreground)
        pd::critical_error("Cannot load map", "Not all layers were specified");
    
    m_blocks = new pd::block*[m_width * m_height];

    pd::xml_element tileset = root.first_child("tileset");
    std::string texture_filename = tileset.first_child().attr("source");
    if (texture_filename.size() > 3 &&
        texture_filename.substr(0, 3) == "../")
        texture_filename = texture_filename.substr(3);
    m_tileset = pd::get_resource<pd::texture>(texture_filename);
    tile_id_t i = (tile_id_t)pd::lexical_cast<int>(tileset.attr("firstgid"));
    for (int y = 0; y < m_tileset->height(); y += m_tile_height) {
        for (int x = 0; x < m_tileset->width(); x += m_tile_width) {
            pd::texture *tile = m_tileset->slice(x, y, m_tile_width, m_tile_height);
            m_tiles[i++] = tile;
        }
    }

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            tile_id_t tile = foreground[(y * m_width) + x];
            pd::block *block = tile ? new pd::block(this, tile, x, y) : 0;
            m_blocks[(y * m_width) + x] = block;
        }
    }
 
    delete[] foreground;
}

pd::map::~map()
{
    for (int i = 0; i < (m_width * m_height); i++)
        delete m_blocks[i];

    delete[] m_background;
    delete[] m_blocks;
}

void pd::map::get_corner_tiles(const pd::aabb &bb, int *left_tile,
                               int *right_tile, int *top_tile,
                               int *bottom_tile) const
{
    *left_tile = (int)pd::floor(bb.left() / m_tile_width);
    *right_tile = (int)pd::ceil(bb.right() / m_tile_width) - 1;
    *top_tile = (int)pd::floor(bb.top() / m_tile_height);
    *bottom_tile = (int)pd::ceil(bb.bottom() / m_tile_height) - 1;
}

pd::tile_collision_flag pd::map::get_collision(int x, int y) const
{
    const pd::block *block = get_block(x, y);
    if (!block)
        return passable;
    return block->collision();
}

void pd::map::draw_tile(int x, int y, pd::map::tile_id_t tile) const
{
    if (!tile)
        return;
    std::map<tile_id_t, pd::texture *>::const_iterator iter = m_tiles.find(tile);
    assert(iter != m_tiles.end());
    pd::draw_quad(iter->second, pd::vec2((float)x * m_tile_width,
                                          (float)y * m_tile_height));
}

void pd::map::draw_tile_bounds() const
{
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            pd::tile_collision_flag c = get_collision(x, y);
            pd::draw_debug_box(pd::vec2(x * m_tile_width, y * m_tile_height),
                               (float)m_tile_width, (float)m_tile_height,
                               c ? 0xb5cf2faa : 0x33333311);
        }
    }
}

void pd::map::update(pd::timedelta_t dt)
{
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            pd::block *block = get_block(x, y);
            if (block)
                block->update(dt);
        }
    }
}

void pd::map::draw() const
{
    pd::clear_screen(m_background_color);
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            draw_tile(x, y, get_bg(x, y));
            const pd::block *block = get_block(x, y);
            if (block)
                block->draw();
        }
    }
}

pd::block::block(pd::map *map, pd::map::tile_id_t tile, int x, int y)
{
    m_map = map;
    m_tile = tile;
    m_x = x;
    m_y = y;
    m_transition = 0;
}

pd::aabb pd::block::bounding_box() const
{
    pd::vec2 pos(m_map->tile_width() * m_x, m_map->tile_height() * m_y);
    pd::vec2 size(m_map->tile_width(), m_map->tile_height());
    return pd::aabb(pos, pos + size);
}

pd::tile_collision_flag pd::block::collision() const
{
    if (m_tile == 0)
        return pd::passable;
    return pd::impassable;
}

pd::map::tile_id_t pd::block::tile_by_name(const std::string &name)
{
    pd::unordered_map<std::string, int>::iterator iter;
    iter = pd::config::blocks.aliases.find(name);
    assert(iter != pd::config::blocks.aliases.end());
    return static_cast<pd::map::tile_id_t>(iter->second);
}

void pd::block::start_transition(pd::map::tile_id_t new_tile)
{
    pd::map::tile_id_t old_tile = m_tile;

    m_tile = new_tile;
    delete m_transition;
    m_transition = 0;

    pd::unordered_map<int, pd::config::block_def>::iterator iter;
    iter = pd::config::blocks.defs.find(old_tile);

    if (iter == pd::config::blocks.defs.end())
        return;

    pd::unordered_map<int, pd::config::animation_config>::iterator titer;
    titer = iter->second.transitions.find(new_tile);

    if (titer == iter->second.transitions.end())
        return;

    m_transition = new pd::animation(titer->second);
}

void pd::block::update(pd::timedelta_t dt)
{
    if (!m_transition)
        return;

    if (m_transition->update(dt)) {
        delete m_transition;
        m_transition = 0;
    }
}

void pd::block::draw() const
{
    if (m_transition)
        m_transition->draw();
    else
        m_map->draw_tile(m_x, m_y, m_tile);
}
