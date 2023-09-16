#pragma once

#include "../part.h"

namespace godot
{
class Wheel : public Part
{
    GDCLASS(Wheel, Part);

public:
    Wheel();
    ~Wheel();

protected:
    static void _bind_methods();

public:
    virtual void _process(double delta) override;
};
}