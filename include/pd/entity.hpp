#ifndef _INC_PD_ENTITY_HPP_
#define _INC_PD_ENTITY_HPP_
#include <pd/pd.hpp>
#include <Box2D/Box2D.h>

namespace pd {

    class texture;

    class entity {
    public:
        entity(b2World *world, float x = 0.0f, float y = 0.0f);
        virtual ~entity() {}

        float x() const { return m_x; }
        void x(float val) { m_x = val; }
        float y() const { return m_y; }
        void y(float val) { m_y = val; }
        void move(float x, float y);

        virtual void update(float dt) = 0;
        virtual void render(float dt) const = 0;

    private:
        b2World *m_world;
        float m_x;
        float m_y;
    };
}

#endif
