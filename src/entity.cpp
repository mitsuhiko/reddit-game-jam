#include <pd/entity.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>
#include <pd/game.hpp>
#include <pd/game_session.hpp>

pd::entity::entity(pd::game_session *session, float x, float y, float width,
                   float height, float density, float friction, bool locked_rotation)
{
    m_session = session;
    m_world = session->box2d_world();
    m_flipped = false;
    session->add_entity(this);

    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.position.Set(pd::pixel_to_meter(x), pd::pixel_to_meter(y));
    bodydef.fixedRotation = locked_rotation;
    m_body = m_world->CreateBody(&bodydef);

    b2FixtureDef fixturedef;
    b2PolygonShape dynamicbox;
    dynamicbox.SetAsBox(pd::pixel_to_meter(width / 2),
                        pd::pixel_to_meter(height / 2));
    fixturedef.shape = &dynamicbox;
    fixturedef.density = density;
    fixturedef.friction = friction;
    m_body->CreateFixture(&fixturedef);
}

pd::entity::~entity()
{
    m_world->DestroyBody(m_body);
}

void pd::entity::move(float dx, float dy)
{
    if (dx < -0.0001f)
        m_flipped = true;
    else if (dx > 0.0001)
        m_flipped = false;
    m_body->ApplyForce(b2Vec2(dx, dy), m_body->GetWorldCenter());
}

void pd::entity::render(float dt) const
{
    pd::push_matrix();
    pd::translate(x(), y());
    local_render(dt);
    pd::pop_matrix();
}