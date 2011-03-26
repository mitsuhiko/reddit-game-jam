#include <pd/entity.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>

pd::entity::entity(b2World *world, float x, float y, float sizex, float sizey, float weight, float friction)
{
    m_world = world;
    m_x = x;
    m_y = y;
    build_box2d_object();
}

void pd::entity::move(float x, float y)
{
    m_x += x;
    m_y += y;
}

void pd::entity::build_box2d_object()
{
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.position.Set(m_x, m_y);
    m_body = m_world->CreateBody(&bodydef);
    b2PolygonShape dynamicbox;
    dynamicbox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixturedef;
    fixturedef.shape = &dynamicbox;
    fixturedef.density = 1.0f;
    fixturedef.friction = 0.3f;
    m_body->CreateFixture(&fixturedef);
}

void pd::entity::sync()
{
    m_x = m_body->GetPosition().x;
    m_y = m_body->GetPosition().y;
}
