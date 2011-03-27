#ifndef _INC_PD_ENTITY_HPP_
#define _INC_PD_ENTITY_HPP_
#include <pd/pd.hpp>
#include <pd/math.hpp>

namespace pd {

    class texture;
    class game_session;

    class entity {
    public:
        enum stance_type {
            neutral_stance,
            thermal_stance,
            electromagnetic_stance,
            kinetic_stance
        };

        entity(pd::game_session *session, float x = 0.0f, float y = 0.0f, float width = 0.0f,
               float height = 0.0f, float base_offset = 0.0f, float density = 0.0f,
               float friction = 0.0f, bool locked_rotation = false);
        virtual ~entity();

        float x() const { return pd::meter_to_pixel(m_body->GetPosition().x); }
        float y() const { return pd::meter_to_pixel(m_body->GetPosition().y); }
        float rotation() const { return pd::rad_to_deg(m_body->GetAngle()); }
        void rotation(float angle) { m_body->SetTransform(m_body->GetPosition(), pd::deg_to_rad(angle)); }
        bool locked_rotation() const { return m_body->IsFixedRotation(); }
        void locked_rotation(bool val) { m_body->SetFixedRotation(val); }

        float width() const { return m_width; }
        float height() const { return m_height; }
        float base_offset() const { return m_base_offset; }
        void base_offset(float val) { m_base_offset = val; }
        b2Body * body() const { return m_body; }
        void body(b2Body * val) { m_body = val; }
        float density() const { return m_fixture->GetDensity(); }
        void density(float val) { m_fixture->SetDensity(val); }
        float friction() const { return m_fixture->GetFriction(); }
        void friction(float val) { m_fixture->SetFriction(val); }
        bool flipped() const { return m_flipped; }
        void flipped(bool val) { m_flipped = val; }
        bool airborne() const;
        pd::vec2 linear_velocity() const;

        stance_type stance() const { return m_stance; }
        void stance(stance_type val) { m_stance = val; }

        void apply_force(float x, float y);
        void apply_impulse(float x, float y);

        virtual void update(float dt) = 0;  
        virtual void render(float dt) const;
        virtual void local_render(float dt) const = 0;

    private:
        pd::game_session *m_session;
        b2World *m_world;
        b2Body *m_body;
        b2Fixture *m_fixture;
        stance_type m_stance;
        float m_width;
        float m_height;
        float m_base_offset;
        bool m_flipped;
    };
}

#endif
