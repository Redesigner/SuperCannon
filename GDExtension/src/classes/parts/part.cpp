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
    BIND_PROPERTY(Variant::NODE_PATH, attachmentPath, Part);
}

void Part::_ready()
{
    // Don't try to get the node path if we're in editor, it might not have been defined yet
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    ASSIGN_NODE(_attachment, RigidBody3D, _attachmentPath);
}

void Part::power(float amount)
{
}

RigidBody3D * Part::get_attachment() const
{
    return _attachment;
}
