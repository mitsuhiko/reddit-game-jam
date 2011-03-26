#include <pd/entity.hpp>
#include <pd/texture.hpp>
#include <pd/drawtools.hpp>

pd::entity::entity(b2World *world, float x, float y, float width, float height, float density, float friction)
{
    m_world = world;
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_density = density;
    m_friction = friction;

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
    bodydef.position.Set(ptm(m_x), ptm(m_y));
    m_body = m_world->CreateBody(&bodydef);
    b2PolygonShape dynamicbox;
    dynamicbox.SetAsBox(ptm(m_width / 2), ptm(m_height / 2));
    b2FixtureDef fixturedef;
    fixturedef.shape = &dynamicbox;
    fixturedef.density = m_density;
    fixturedef.friction = m_friction;
    m_body->CreateFixture(&fixturedef);
}

void pd::entity::sync()
{
    m_x = mtp(m_body->GetPosition().x);
    m_y = mtp(m_body->GetPosition().y);
}
