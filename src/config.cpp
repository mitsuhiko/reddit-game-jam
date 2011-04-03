#include <pd/config.hpp>
#include <pd/xml.hpp>
#include <pd/lexical_cast.hpp>

pd::config::_player pd::config::player;
pd::config::_world pd::config::world;


static void load_world_config()
{
    pd::config::_world &cfg = pd::config::world;
    pd::xml_document doc("config/world.xml");
    pd::xml_element physics = doc.root().first_child("physics");

    cfg.gravity_acceleration = pd::lexical_cast<float>(physics.attr("gravity-acceleration"));
    cfg.max_fall_speed = pd::lexical_cast<float>(physics.attr("max-fall-speed"));
}

static void load_player_config()
{
    pd::config::_player &cfg = pd::config::player;
    pd::xml_document doc("config/player.xml");
    pd::xml_element movement = doc.root().first_child("movement");
    pd::xml_element jumping = doc.root().first_child("jumping");

    cfg.max_movement_speed = pd::lexical_cast<float>(movement.attr("max-speed"));
    cfg.max_kinetic_movement_speed = pd::lexical_cast<float>(movement.attr("max-kinetic-speed"));
    cfg.movement_acceleration = pd::lexical_cast<float>(movement.attr("acceleration"));
    cfg.kinetic_movement_acceleration = pd::lexical_cast<float>(movement.attr("kinetic-acceleration"));
    cfg.friction = pd::lexical_cast<float>(movement.attr("friction"));
    cfg.max_jump_time = pd::lexical_cast<float>(jumping.attr("max-time"));
    cfg.jump_launch_velocity = pd::lexical_cast<float>(jumping.attr("launch-velocity"));
    cfg.jump_control_power = pd::lexical_cast<float>(jumping.attr("control-power"));
}

void pd::config::load()
{
    load_world_config();
    load_player_config();
}