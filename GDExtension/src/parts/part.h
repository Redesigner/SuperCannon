#pragma once

#include <godot_cpp/classes/node3d.hpp>

namespace godot
{
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

};
}