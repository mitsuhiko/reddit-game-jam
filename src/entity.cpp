#include <pd/entity.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>
#include <pd/config.hpp>


pd::entity::entity(pd::game_session *session, const glm::vec2 &pos, float width,
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
    return pd::aabb(m_pos, m_pos + glm::vec2(m_width, m_height));
}

void pd::entity::handle_collisions()
{
    glm::vec2 old_pos = pos();
    pd::aabb bb = bounding_box();
    float tile_width = (float)session()->map()->tile_width();
    float tile_height = (float)session()->map()->tile_height();

    int left_tile = (int)std::floor(bb.left() / tile_width);
    int right_tile = (int)std::ceil(bb.right() / tile_width) - 1;
    int top_tile = (int)std::floor(bb.top() / tile_height);
    int bottom_tile = (int)std::ceil(bb.bottom() / tile_height) - 1;

    m_on_ground = false;

    for (int y = top_tile; y <= bottom_tile; y++) {
        for (int x = left_tile; x <= right_tile; x++) {
            const pd::block *block = session()->map()->get_block(x, y);

            if (!block || block->collision() == pd::block::passable)
                continue;

            pd::aabb block_bb = block->bounding_box();
            glm::vec2 depth = bounding_box().intersection_depth(block_bb);
            if (depth == glm::vec2())
                continue;

            glm::vec2 abs_depth = glm::abs(depth);
            glm::vec2 correction;
            if (abs_depth.y < abs_depth.x ||
                block->collision() == pd::block::semi_passable) {
                if (m_previous_bottom <= block_bb.top())
                    m_on_ground = true;

                if (m_on_ground ||
                    block->collision() == pd::block::impassable)
                    correction.y = depth.y;
            } else if (block->collision() == pd::block::impassable) {
                correction.x = depth.x;
            }

            if (correction != glm::vec2())
                pos(pos() + correction);
        }
    }

    m_previous_bottom = bounding_box().bottom();
}
