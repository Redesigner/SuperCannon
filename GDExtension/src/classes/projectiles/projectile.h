#pragma once

#include <godot_cpp/classes/rigid_body3d.hpp>

namespace godot
{
class CannonBody;
class Projectile : public RigidBody3D
{
    GDCLASS(Projectile, RigidBody3D);

public:
    Projectile();
    ~Projectile();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    virtual void _integrate_forces(PhysicsDirectBodyState3D *state) override;

    void collide(Node *body);

    virtual void on_collision(Node3D *other_body, Vector3 position, Vector3 normal);

    void set_cannon_owner(CannonBody *owner);

    CannonBody *get_cannon_owner() const;

private:
    bool _collided_this_frame = false;

    CannonBody *_cannon_owner = nullptr;
};
}