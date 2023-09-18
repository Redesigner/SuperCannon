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
    virtual void _ready() override;

    virtual void _physics_process(double delta) override;

private:
    float get_friction_force() const;

    float get_applied_torque_force() const;

    float get_suspension_force() const;

    Vector3 get_total_forces() const;

    float get_distance_to_ground() const;



    bool _is_grounded;

    // Reusable params for the raycast, so we don't have to reallocate the array every tick
    Ref<class PhysicsRayQueryParameters3D> _rayQueryParameters;

    // Sliding friction coeffecient between the wheel and the floor. This may be changed for different surfaces. Unitless
    DECLARE_PROPERTY(float, frictionCoeffecient)
    // Radius of the wheel, in m
    DECLARE_PROPERTY(float, radius);

    // How far from the original location the wheel can travel, in m
    DECLARE_PROPERTY(float, maxSuspensionLength);
    // The spring constant, in N/m
    DECLARE_PROPERTY(float, springConstant);
};
}