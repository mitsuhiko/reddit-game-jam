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
        enum stance_type {
            neutral_stance,
            thermal_stance,
            electromagnetic_stance,
            kinetic_stance
        };
        enum damage_type {
            true_damage,
            thermal_damage,
            electromagnetic_damage,
            kinetic_damage
        };

        entity(pd::game_session *session, float x = 0.0f, float y = 0.0f, float width = 0.0f,
               float height = 0.0f);
        virtual ~entity();

        const glm::vec2 &pos() const { return m_pos; }
        const float x() const { return m_pos[0]; }
        const float y() const { return m_pos[1]; }
        void velocity(float dx, float dy) { velocity(glm::vec2(dx, dy)); }
        void velocity(const glm::vec2 &vec) { m_velocity = vec; }
        const glm::vec2 &velocity() const { return m_velocity; }
        pd::aabb bounding_box() const;
        float rotation() const { return m_rotation; }
        void rotation(float angle) { m_rotation = angle; }
        void move(float dx, float dy) { move(glm::vec2(dx, dy)); }
        void move(const glm::vec2 &vec) { m_pos += vec; }

        float width() const { return m_width; }
        float height() const { return m_height; }
        float hovering() const { return m_hovering; }
        void hovering(float val) { m_hovering = val; }
        virtual bool flipped() const;

        bool collides_right() const;
        bool collides_left() const;
        bool airborne() const;

        float health() const { return m_health; }
        void health(float val) { m_health = val; }
        bool dead() const { return m_health <= 0.0f; }
        virtual void take_damage(float val, damage_type type);

        stance_type stance() const { return m_stance; }
        void stance(stance_type val) { m_stance = val; }

        pd::game_session *session() { return m_session; }
        const pd::game_session *session() const { return m_session; }

        void apply_gravity(pd::timedelta_t dt);

        virtual void update(pd::timedelta_t dt) = 0;
        virtual void apply_physics(pd::timedelta_t dt);
        virtual void render(pd::timedelta_t dt) const;
        virtual void local_render(pd::timedelta_t dt) const = 0;

    private:
        pd::game_session *m_session;
        glm::vec2 m_velocity;
        glm::vec2 m_pos;
        float m_rotation;
        stance_type m_stance;
        float m_width;
        float m_height;
        float m_health;
        float m_hovering;
        float m_air_time;
        mutable bool m_was_flipped;
    };
}

#endif
