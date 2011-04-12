#ifndef _INC_PD_ENTITY_HPP_
#define _INC_PD_ENTITY_HPP_
#include <pd/pd.hpp>
#include <pd/math.hpp>
#include <pd/aabb.hpp>
#include <pd/collisions.hpp>
#include <vector>


namespace pd {

    class texture;
    class game_session;

    class entity {
    public:
        entity(pd::game_session *session, const pd::vec2 &pos);
        virtual ~entity();

        void pos(const pd::vec2 &pos) { m_pos = pos; }
        const pd::vec2 &pos() const { return m_pos; }
        void move(const pd::vec2 &vec) { m_pos += vec; }
        bool on_ground() const { return m_on_ground; }

        virtual float width() const = 0;
        virtual float height() const = 0;

        pd::game_session *session() { return m_session; }
        const pd::game_session *session() const { return m_session; }

        int move_collision_checked(const pd::vec2 &delta);
        pd::aabb bounding_box() const;

        virtual void handle_event(SDL_Event &evt);
        virtual void update(pd::timedelta_t dt) = 0;
        virtual void draw() const = 0;

    private:
        pd::game_session *m_session;
        pd::vec2 m_pos;
        bool m_on_ground;
        float m_previous_bottom;
    };
}

#endif
