#include "part.h"

#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Part::Part()
{
    _attachment = nullptr;
}

Part::~Part()
{
}

void Part::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, attachment_path, Part);

    ClassDB::bind_method(D_METHOD("power", "amount"), &Part::power);
    ClassDB::bind_method(D_METHOD("steer", "amount"), &Part::steer);
    ClassDB::bind_method(D_METHOD("control", "input"), &Part::control);
    ClassDB::bind_method(D_METHOD("activate"), &Part::activate);
}

void Part::_ready()
{
    // Don't try to get the node path if we're in editor, it might not have been defined yet
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    ASSIGN_NODE(_attachment, RigidBody3D, _attachment_path);
}

void Part::power(float amount)
{
}

void Part::steer(float amount)
{
}

void Part::control(Vector2 input)
{
}

void Part::activate()
{
}

RigidBody3D * Part::get_attachment() const
{
    return _attachment;
}
