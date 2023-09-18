#pragma once

#include <godot_cpp/classes/node3d.hpp>

#include "../../macros/class_macros.h"

namespace godot
{
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

    virtual void _ready() override;

protected:
    RigidBody3D * get_attachment() const;

private:
    DECLARE_PROPERTY(NodePath, attachment_path);

    RigidBody3D *_attachment;
};
}