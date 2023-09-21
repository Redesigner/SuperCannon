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

    virtual void power(float amount) override;

    virtual void steer(float amount) override;

private:
    Vector3 get_friction_force(double delta, float normal_force) const;

    float get_applied_torque_force_magnitude() const;

    /// @param spring_compression How far the spring is being compressed, in m
    /// @returns Magnitude of orce exerted by the suspension on the body it's attached to
    float get_suspension_force_magnitude(float spring_compression) const;

    /// @brief Get the sum of forces being applied by the wheel on the attachment body
    /// @param delta Timestep, in s
    /// @return Total forces, in N
    Vector3 get_total_forces(double delta);

    /// @brief Get the compression of the suspension spring, and update the floor normal
    /// @returns How far the suspension is compressed (magnitude)
    /// @param touching_ground_out set false if the wheel is not touching the ground
    float get_suspension_compression(bool &touching_ground_out);

    // get an approximation of the damping force at the wheel's location
    float get_damping_force() const;


    class MeshInstance3D *_wheel_mesh;
    DECLARE_PROPERTY(NodePath, wheel_mesh_path);

    class MeshInstance3D *_force_mesh;
    DECLARE_PROPERTY(NodePath, force_mesh_path);

    // Reusable params for the raycast, so we don't have to reallocate the array every tick
    Ref<class PhysicsRayQueryParameters3D> _rayQueryParameters;

    // Sliding friction coeffecient between the wheel and the floor. This may be changed for different surfaces. Unitless
    DECLARE_PROPERTY(float, friction_coeffecient)
    // Radius of the wheel, in m
    DECLARE_PROPERTY(float, radius);

    // How far from the original location the wheel can travel, in m
    // For now, this is basically how far the wheel can be pushed UP relative to its initial position
    DECLARE_PROPERTY(float, max_suspension_length);
    // The spring constant, in N/m
    DECLARE_PROPERTY(float, spring_constant);
    // Damping factor, unitless
    DECLARE_PROPERTY(float, damping);

    // The torque of the wheel, in Nm
    DECLARE_PROPERTY(float, torque);

    // The furthest the wheel can turng, in degrees
    DECLARE_PROPERTY(float, max_turning_angle);

    // Current power of the wheel
    // In a range [-1, 1], with negative being backwards
    DECLARE_PROPERTY(float, power);

    float _previous_distance_to_ground = 0.f;

    float _suspension_velocity = 0.f;

    float _rotational_velocity = 0.f;

    Vector3 _previous_position = Vector3(0.0f, 0.0f, 0.0f);

    Vector3 _velocity = Vector3(0.0f, 0.0f, 0.0f);

    Vector3 _floor_normal = Vector3(0.0f, 0.0f, 0.0f);
};
}