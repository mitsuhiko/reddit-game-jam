#ifndef _INC_PD_ENTITY_HPP_
#define _INC_PD_ENTITY_HPP_
#include <pd/pd.hpp>
#include <pd/math.hpp>
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

        float x() const { return m_x; }
        float y() const { return m_y; }
        float rotation() const { return m_rotation; }
        void rotation(float angle) { m_rotation = angle; }

        float width() const { return m_width; }
        float height() const { return m_height; }
        bool flipped() const { return m_flipped; }
        void flipped(bool val) { m_flipped = val; }
        bool airborne() const;

        float health() const { return m_health; }
        void health(float val) { m_health = val; }
        bool dead() const { return m_health <= 0.0f; }
        virtual void take_damage(float val, damage_type type);

        stance_type stance() const { return m_stance; }
        void stance(stance_type val) { m_stance = val; }

        pd::game_session *session() { return m_session; }
        const pd::game_session *session() const { return m_session; }

        virtual void update(float dt) = 0;  
        virtual void render(float dt) const;
        virtual void local_render(float dt) const = 0;

    private:
        pd::game_session *m_session;
        float m_x;
        float m_y;
        float m_rotation;
        stance_type m_stance;
        float m_width;
        float m_height;
        float m_health;
        bool m_flipped;
    };
}

#endif
