#ifndef _INC_PD_ENTITY_HPP_
#define _INC_PD_ENTITY_HPP_
#include <pd/pd.hpp>
#include <pd/math.hpp>

namespace pd {

    class texture;
    class game_session;

    class entity {
    public:
        entity(pd::game_session *session, float x = 0.0f, float y = 0.0f, float width = 0.0f,
            float height = 0.0f, float density = 0.0f, float friction = 0.0f);
        virtual ~entity();

        float x() const { return pd::meter_to_pixel(m_body->GetPosition().x); }
        float y() const { return pd::meter_to_pixel(m_body->GetPosition().y); }
        float width() const { return m_width; }
        float height() const { return m_height; }
        float density() const { return m_fixture->GetDensity(); }
        void density(float val) { m_fixture->SetDensity(val); }
        float friction() const { return m_fixture->GetFriction(); }
        void friction(float val) { m_fixture->SetFriction(val); }
        virtual void update(float dt) = 0;
        virtual void render(float dt) const = 0;

    private:
        b2World *m_world;
        b2Body *m_body;
        b2Fixture *m_fixture;
        float m_width;
        float m_height;
    };
}

#endif
