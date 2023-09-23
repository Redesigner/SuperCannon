#pragma once

#include <godot_cpp/classes/rigid_body3d.hpp>

#include <vector.hpp>

namespace godot
{
class CannonBody : public RigidBody3D
{
    GDCLASS(CannonBody, RigidBody3D);

public:
    CannonBody();
    ~CannonBody();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

private:
    
};
}