#include "cannon_body.h"

#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../parts/part.h"
#include "../controller/controller.h"

using namespace godot;

CannonBody::CannonBody()
{
    _attached_parts = std::vector<Part *>();
    _spawn_transform = Transform3D();
}

CannonBody::~CannonBody()
{
}

void CannonBody::_bind_methods()
{
}

void CannonBody::_ready()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    _spawn_transform = get_global_transform();

    TypedArray<Node> children = get_children();
    for (int i = 0; i < children.size(); i++)
    {
        if (Part *part = Object::cast_to<Part>(children[i]))
        {
            _attached_parts.push_back(part);
        }
    }
}


void CannonBody::_physics_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    if (get_global_position().y <= -100.0f)
    {
        UtilityFunctions::print("[CannonBody] fell below the world, resetting.");
        set_linear_velocity(Vector3());
        set_angular_velocity(Vector3());
        set_global_transform(_spawn_transform);
    }
}


void CannonBody::take_control(Controller *controller)
{
    _controller = controller;
}


void CannonBody::power(float input)
{
    for (Part *part : _attached_parts)
    {
        part->power(input);
    }
}


void CannonBody::steer(float input)
{
    for (Part *part : _attached_parts)
    {
        part->steer(input);
    }
}


void CannonBody::control(Vector2 input)
{
    for (Part *part : _attached_parts)
    {
        part->control(input);
    }
}


void CannonBody::activate()
{
    for (Part *part : _attached_parts)
    {
        part->activate();
    }
}

Controller *CannonBody::get_controller() const
{
    return _controller;
}

void CannonBody::_notification(int p_what)
{
}
