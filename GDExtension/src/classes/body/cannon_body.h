#pragma once

#include <godot_cpp/classes/rigid_body3d.hpp>

#include <vector>

namespace godot
{
class Part;
class Controller;
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

    virtual void _physics_process(double delta) override;

    void take_control(Controller *controller);

    void power(float input);

    void steer(float input);

    void control(Vector2 input);

    void activate();

    Controller * get_controller() const;

protected:
    void _notification(int p_what);

private:
    // use ref counted instead of pointer?
    std::vector<Part *> _attached_parts;

    Transform3D _spawn_transform;

    Controller *_controller = nullptr;
};
}