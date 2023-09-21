#pragma once

#include <godot_cpp/classes/rigid_body3d.hpp>

namespace godot
{
class Projectile : public RigidBody3D
{
    GDCLASS(Projectile, RigidBody3D);

public:
    Projectile();
    ~Projectile();

protected:
    static void _bind_methods();

public:

};
}