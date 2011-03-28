#ifndef _INC_PD_ENTITY_HPP_
#define _INC_PD_ENTITY_HPP_
#include <pd/pd.hpp>
#include <pd/math.hpp>
#include <pd/aabb.hpp>
#include <vector>


namespace pd {

    class texture;
    class game_session;

    class entity {
    public:
        entity(pd::game_session *session, const glm::vec2 &pos, float width = 0.0f,
               float height = 0.0f);
        virtual ~entity();

        void pos(const glm::vec2 &pos) { m_pos = pos; }
        const glm::vec2 &pos() const { return m_pos; }
        void move(const glm::vec2 &vec) { m_pos += vec; }

        float width() const { return m_width; }
        float height() const { return m_height; }

        pd::game_session *session() { return m_session; }
        const pd::game_session *session() const { return m_session; }

        virtual void update(pd::timedelta_t dt) = 0;
        virtual void render(pd::timedelta_t dt) const = 0;

        pd::aabb bounding_box() const;

    private:
        pd::game_session *m_session;
        glm::vec2 m_pos;
        float m_width;
        float m_height;
    };
}

#endif
