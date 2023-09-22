#pragma once

#include "projectile.h"

#include "../../macros/class_macros.h"

namespace godot
{
class PackedScene;
class CannonSpawnerProjectile : public Projectile
{
    GDCLASS(CannonSpawnerProjectile, Projectile);

public:
    CannonSpawnerProjectile();
    ~CannonSpawnerProjectile();

protected: 
    static void _bind_methods();

public:
    virtual void _ready() override;

    virtual void on_collision(Node3D *other_body, Vector3 position, Vector3 normal) override;

private:
    Ref<PackedScene> _cannon_scene;
    DECLARE_PROPERTY(String, cannon_scene_path);
};
}