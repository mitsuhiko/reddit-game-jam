#ifndef _INC_PD_ENTITY_HPP_
#define _INC_PD_ENTITY_HPP_
#include <pd/pd.hpp>
#include <Box2D/Box2D.h>

// Pixels to meters translation.
#define ptm(a) (a / 30)
#define mtp(a) (a * 30)

namespace pd {

    class texture;

    class entity {
    public:
        entity(b2World *world, float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f, float density = 0.0f, float friction = 0.0f);
        virtual ~entity() {}

        float x() const { return m_x; }
        void x(float val) { m_x = val; }
        float y() const { return m_y; }
        void y(float val) { m_y = val; }
        float width() const { return m_width; }
        void width(float val) { m_width = val; }
        float height() const { return m_height; }
        void height(float val) { m_height = val; }
        float density() const { return m_density; }
        void density(float val) { m_density = val; }
        float friction() const { return m_friction; }
        void friction(float val) { m_friction = val; }
        void move(float x, float y);
        void build_box2d_object();
        /* Syncs box2d body position with entity position. Should be called each frame. */
        void sync();

        virtual void update(float dt) = 0;
        virtual void render(float dt) const = 0;

    private:
        b2World *m_world;
        b2Body *m_body;
        float m_x;
        float m_y;
        float m_width;
        float m_height;
        float m_density;
        float m_friction;
    };
}

#endif
