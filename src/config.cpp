#include <pd/config.hpp>
#include <pd/xml.hpp>

pd::config::_world pd::config::world;
pd::config::_camera pd::config::camera;
pd::config::_console pd::config::console;
pd::config::_player pd::config::player;
pd::config::_kinetic_enemy pd::config::kinetic_enemy;


static void load_animation_config(pd::config::animation_config &cfg,
                                  pd::xml_element &animation)
{
    cfg.texture = animation.attr("texture");
    cfg.frames = animation.attr<int>("frames");
    cfg.speed = animation.attr<float>("speed");
}

static void load_vector_config(pd::vec2 &vec, pd::xml_element &element)
{
    vec.x = element.attr<float>("x");
    vec.y = element.attr<float>("y");
}

static void load_world_config()
{
    static const char *filename = "config/world.xml";
    PD_LOG("Loading world config from " << filename);

    pd::config::_world &cfg = pd::config::world;
    pd::xml_document doc(filename);
    pd::xml_element physics = doc.root().first_child("physics");

    cfg.gravity_acceleration = physics.attr<float>("gravity-acceleration");
    cfg.max_fall_speed = physics.attr<float>("max-fall-speed");
}

static void load_camera_config()
{
    static const char *filename = "config/camera.xml";
    PD_LOG("Loading camera config from " << filename);

    pd::config::_camera &cfg = pd::config::camera;
    pd::xml_document doc(filename);
    pd::xml_element movement = doc.root().first_child("movement");

    cfg.adjust_time = movement.attr<float>("adjust-time");
}

static void load_console_config()
{
    static const char *filename = "config/console.xml";
    PD_LOG("Loading console config from " << filename);

    pd::config::_console &cfg = pd::config::console;
    pd::xml_document doc(filename);
    pd::xml_element settings = doc.root().first_child("settings");

    cfg.max_lines = settings.attr<int>("max-lines");
    cfg.print_to_stderr = settings.attr<bool>("print-to-stderr");
}

static void load_player_config()
{
    static const char *filename = "config/player.xml";
    PD_LOG("Loading player config from " << filename);

    pd::config::_player &cfg = pd::config::player;
    pd::xml_document doc(filename);
    pd::xml_element root = doc.root();
    pd::xml_element movement = root.first_child("movement");
    pd::xml_element jumping = root.first_child("jumping");
    pd::xml_element animations = root.first_child("animations");
    pd::xml_element offsets = root.first_child("offsets");

    cfg.width = root.attr<float>("width");
    cfg.height = root.attr<float>("height");

    cfg.max_movement_speed = movement.attr<float>("max-speed");
    cfg.max_kinetic_movement_speed = movement.attr<float>("max-kinetic-speed");
    cfg.movement_acceleration = movement.attr<float>("acceleration");
    cfg.kinetic_movement_acceleration = movement.attr<float>("kinetic-acceleration");
    cfg.air_friction = movement.attr<float>("air-friction");
    cfg.friction = movement.attr<float>("friction");

    cfg.jump_impulse = jumping.attr<float>("impulse");

    for (pd::xml_element animation = animations.first_child();
         animation; animation = animation.next_sibling()) {
        std::string animation_name = animation.attr("name");
        if (animation_name == "thermal_idle")
            load_animation_config(cfg.thermal_idle, animation);
        else if (animation_name == "kinetic_idle")
            load_animation_config(cfg.kinetic_idle, animation);
        else if (animation_name == "electromagnetic_idle")
            load_animation_config(cfg.electromagnetic_idle, animation);
        else if (animation_name == "flamethrower")
            load_animation_config(cfg.flamethrower, animation);
        else if (animation_name == "ice-spray")
            load_animation_config(cfg.ice_spray, animation);
    }

    for (pd::xml_element offset = offsets.first_child();
         offset; offset = offset.next_sibling()) {
        std::string offset_name = offset.attr("name");
        if (offset_name == "thermal")
            load_vector_config(cfg.thermal_offset, offset);
        else if (offset_name == "kinetic")
            load_vector_config(cfg.kinetic_offset, offset);
        else if (offset_name == "electromagnetic")
            load_vector_config(cfg.electromagnetic_offset, offset);
        else if (offset_name == "flamethrower")
            load_vector_config(cfg.flamethrower_offset, offset);
        else if (offset_name == "ice-spray")
            load_vector_config(cfg.ice_spray_offset, offset);
    }
}

static void load_kinetic_enemy_config()
{
    static const char *filename = "config/kinetic_enemy.xml";
    PD_LOG("Loading kinetic enemy config from " << filename);

    pd::config::_kinetic_enemy &cfg = pd::config::kinetic_enemy;
    pd::xml_document doc(filename);
    pd::xml_element root = doc.root();
    pd::xml_element behavior = root.first_child("behavior");
    pd::xml_element animations = root.first_child("animations");
    pd::xml_element offsets = root.first_child("offsets");

    cfg.width = root.attr<float>("width");
    cfg.height = root.attr<float>("height");

    cfg.spawn_fall_speed = behavior.attr<float>("spawn-fall-speed");
    cfg.movement_speed = behavior.attr<float>("movement-speed");
    cfg.dash_speed = behavior.attr<float>("dash-speed");
    cfg.dash_countdown = behavior.attr<float>("dash-countdown");
    cfg.see_distance = behavior.attr<float>("see-distance");

    for (pd::xml_element animation = animations.first_child();
         animation; animation = animation.next_sibling()) {
        std::string animation_name = animation.attr("name");
        if (animation_name == "walk")
            load_animation_config(cfg.walk, animation);
        else if (animation_name == "dash")
            load_animation_config(cfg.dash, animation);
    }

    for (pd::xml_element offset = offsets.first_child();
         offset; offset = offset.next_sibling()) {
        std::string offset_name = offset.attr("name");
        if (offset_name == "default")
            load_vector_config(cfg.default_offset, offset);
        else if (offset_name == "dash")
            load_vector_config(cfg.dash_offset, offset);
    }
}

void pd::config::load()
{
    load_world_config();
    load_camera_config();
    load_console_config();
    load_player_config();
    load_kinetic_enemy_config();
}
