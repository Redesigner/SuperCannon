#pragma once

#include <godot_cpp/classes/node3d.hpp>

#include "../../macros/class_macros.h"

namespace godot
{
class CannonBody;
class NodePath;
class RigidBody3D;

class Part : public Node3D
{
    GDCLASS(Part, Node3D);

public:
    Part();
    ~Part();

protected:
    static void _bind_methods();

public:
    virtual void power(float amount);

    virtual void steer(float amount);

    virtual void control(Vector2 input);

    virtual void activate();

    virtual void _ready() override;

protected:
    CannonBody *get_attachment() const;

private:
    DECLARE_PROPERTY(NodePath, attachment_path);

    CannonBody *_attachment;
};
}