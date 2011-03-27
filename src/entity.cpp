#include <pd/entity.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>

pd::entity::entity(pd::game_session *session, float x, float y, float width,
                   float height, float base_offset, float density,
                   float friction, bool locked_rotation)
{
    m_session = session;
    m_world = session->box2d_world();
    m_flipped = false;
    m_width = width;
    m_height = height;
    m_base_offset = base_offset;
    m_stance = neutral_stance;
    session->add_entity(this);

    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.position.Set(pd::pixel_to_meter(x), pd::pixel_to_meter(y));
    bodydef.fixedRotation = locked_rotation;
    m_body = m_world->CreateBody(&bodydef);

    b2FixtureDef fixturedef;
    b2PolygonShape dynamicbox;
    dynamicbox.SetAsBox(pd::pixel_to_meter(width / 2.0f),
                        pd::pixel_to_meter(height / 2.0f));
    fixturedef.shape = &dynamicbox;
    fixturedef.density = density;
    fixturedef.friction = friction;
    m_body->CreateFixture(&fixturedef);
}

pd::entity::~entity()
{
    m_world->DestroyBody(m_body);
}

bool pd::entity::airborne() const
{
    for (b2ContactEdge* ce = m_body->GetContactList(); ce; ce = ce->next) {
        if (!ce->contact->IsTouching())
            continue;
        b2WorldManifold worldManifold;
        ce->contact->GetWorldManifold(&worldManifold);
        if (worldManifold.normal.y < 0.0f)
            return false;
    }
    return true;
}

pd::vec2 pd::entity::linear_velocity() const
{
    return m_body->GetLinearVelocity();
}

void pd::entity::apply_force(float x, float y)
{
    m_body->ApplyForce(b2Vec2(x, y), m_body->GetWorldCenter());
}

void pd::entity::apply_impulse(float x, float y)
{
    m_body->ApplyLinearImpulse(b2Vec2(x, y), m_body->GetWorldCenter());
}

void pd::entity::render(float dt) const
{
    pd::push_matrix();
    pd::translate(x() - width() / 2.0f, y() - height() / 2.0f + m_base_offset);
    if (m_flipped) {
        pd::scale(-1.0f, 1.0f);
        pd::translate(-width(), 0.0f);
    }
    pd::rotate_around_point(rotation(), width() / 2.0f, height() / 2.0f);
    local_render(dt);
    pd::pop_matrix();
}