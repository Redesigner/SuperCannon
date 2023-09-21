#pragma once

#include "../part.h"

namespace godot
{
class Cannon : public Part
{
    GDCLASS(Cannon, Part)

public:
    Cannon();
    ~Cannon();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    virtual void _physics_process(double delta) override;

    virtual void control(Vector2 input) override;

    virtual void activate() override;

private:
    DECLARE_PROPERTY(float, pitch_speed);
    DECLARE_PROPERTY(float, max_pitch);
    DECLARE_PROPERTY(float, min_pitch);

    DECLARE_PROPERTY(float, yaw_speed);

    DECLARE_PROPERTY(float, projectile_velocity);

    Vector2 _control_input;

    Node3D *_barrel = nullptr;
    DECLARE_PROPERTY(NodePath, barrel_path);
    Node3D *_base = nullptr;
    DECLARE_PROPERTY(NodePath, base_path);

    Ref<class PackedScene> _projectile_scene;
    DECLARE_PROPERTY(String, projectile_scene_path);
};
}