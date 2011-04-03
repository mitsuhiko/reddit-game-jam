#include <pd/entity.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/config.hpp>


pd::entity::entity(pd::game_session *session, const pd::vec2 &pos, float width,
                   float height)
{
    m_session = session;
    m_pos = pos;
    m_width = width;
    m_height = height;
    m_previous_bottom = 0.0f;
    m_on_ground = false;
}

pd::entity::~entity()
{
}

pd::aabb pd::entity::bounding_box() const
{
    return pd::aabb(m_pos, m_pos + pd::vec2(m_width, m_height));
}

void pd::entity::handle_collisions()
{
    pd::vec2 old_pos = pos();
    pd::aabb bb = bounding_box();
    const pd::map *map = session()->map();
    float tile_width = (float)map->tile_width();
    float tile_height = (float)map->tile_height();

    int left_tile = (int)pd::floor(bb.left() / tile_width);
    int right_tile = (int)pd::ceil(bb.right() / tile_width) - 1;
    int top_tile = (int)pd::floor(bb.top() / tile_height);
    int bottom_tile = (int)pd::ceil(bb.bottom() / tile_height) - 1;

    m_on_ground = false;

    for (int y = top_tile; y <= bottom_tile; y++) {
        for (int x = left_tile; x <= right_tile; x++) {
            pd::collision_flag collision = map->get_collision(x, y);

            if (collision == pd::passable)
                continue;

            pd::aabb block_bb = map->get_block(x, y)->bounding_box();
            pd::vec2 depth = bounding_box().intersection_depth(block_bb);
            if (depth == pd::vec2())
                continue;

            pd::vec2 abs_depth = pd::abs(depth);
            if (abs_depth.y < abs_depth.x || collision == pd::semi_passable) {
                if (m_previous_bottom <= block_bb.top())
                    m_on_ground = true;
                if (m_on_ground || collision == pd::impassable) {
                    // if we collided with the ground we just realign properly
                    // with the top pixel of the block's bounding box.  Why?
                    // because there will always be a rounding error in the depth
                    // of the intersection and this will cause us to stick in the
                    // ground when gravity is applied.  This is a crude hack but
                    // it works surprisingly well.
                    if (depth.y < 0.0f)
                        pos(pd::vec2(pos().x, block_bb.top() - height()));
                    else
                        move(pd::vec2(0.0f, depth.y));
                }
            } else if (collision == pd::impassable)
                move(pd::vec2(depth.x, 0.0f));
        }
    }

    m_previous_bottom = bounding_box().bottom();
}
