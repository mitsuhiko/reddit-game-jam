#include <pd/config.hpp>
#include <pd/xml.hpp>

pd::config::_player pd::config::player;
pd::config::_world pd::config::world;


static void load_world_config()
{
    PD_LOG("Loading world config from config/world.xml");

    pd::config::_world &cfg = pd::config::world;
    pd::xml_document doc("config/world.xml");
    pd::xml_element physics = doc.root().first_child("physics");

    cfg.gravity_acceleration = physics.attr<float>("gravity-acceleration");
    cfg.max_fall_speed = physics.attr<float>("max-fall-speed");
}

static void load_player_config()
{
    PD_LOG("Loading player config from config/player.xml");

    pd::config::_player &cfg = pd::config::player;
    pd::xml_document doc("config/player.xml");
    pd::xml_element root = doc.root();
    pd::xml_element movement = root.first_child("movement");
    pd::xml_element jumping = root.first_child("jumping");

    cfg.max_movement_speed = movement.attr<float>("max-speed");
    cfg.max_kinetic_movement_speed = movement.attr<float>("max-kinetic-speed");
    cfg.movement_acceleration = movement.attr<float>("acceleration");
    cfg.kinetic_movement_acceleration = movement.attr<float>("kinetic-acceleration");
    cfg.friction = movement.attr<float>("friction");
    cfg.max_jump_time = jumping.attr<float>("max-time");
    cfg.jump_launch_velocity = jumping.attr<float>("launch-velocity");
    cfg.jump_control_power = jumping.attr<float>("control-power");

    pd::xml_element animations = root.first_child("animations");
    for (pd::xml_element animation = animations.first_child();
         animation; animation = animation.next_sibling()) {
        pd::config::animation_config *target;
        std::string animation_name = animation.attr("name");
        if (animation_name == "thermal_idle")
            target = &cfg.thermal_idle;
        else if (animation_name == "kinetic_idle")
            target = &cfg.kinetic_idle;
        else if (animation_name == "electromagnetic_idle")
            target = &cfg.electromagnetic_idle;
        else if (animation_name == "flamethrower")
            target = &cfg.flamethrower;
        else
            continue;

        target->texture = animation.attr("texture");
        target->frames = animation.attr<int>("frames");
        target->speed = animation.attr<float>("speed");
    }
}

void pd::config::load()
{
    load_world_config();
    load_player_config();
}