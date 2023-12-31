#include "wheel.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/world3d.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../../body/cannon_body.h"

using namespace godot;

Wheel::Wheel()
{
    _friction_coeffecient = 0.0f;
    _radius = 0.0f;

    _max_suspension_length = 0.0f;
    _spring_constant = 0.0f;
    _damping = 0.0f;

    _torque = 0.0f;

    _max_turning_angle = 0.0f;

    _power = 0.0f;

    _wheel_mesh = nullptr;
    _force_mesh = nullptr;
}


Wheel::~Wheel()
{
}


void Wheel::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, wheel_mesh_path, Wheel);

    BIND_PROPERTY(Variant::NODE_PATH, force_mesh_path, Wheel);

    BIND_PROPERTY_HINT(Variant::FLOAT, friction_coeffecient, Wheel, PROPERTY_HINT_RANGE, "0, 10");
    BIND_PROPERTY_HINT(Variant::FLOAT, radius, Wheel, PROPERTY_HINT_NONE, "suffix:m");

    BIND_PROPERTY_HINT(Variant::FLOAT, max_suspension_length, Wheel, PROPERTY_HINT_NONE, "suffix:m");
    BIND_PROPERTY_HINT(Variant::FLOAT, spring_constant, Wheel, PROPERTY_HINT_NONE, "suffix:N/m");
    BIND_PROPERTY_HINT(Variant::FLOAT, damping, Wheel, PROPERTY_HINT_RANGE, "0, 1");

    BIND_PROPERTY_HINT(Variant::FLOAT, torque, Wheel, PROPERTY_HINT_RANGE, "0, 100000, 1, suffix:Nm");

    BIND_PROPERTY_HINT(Variant::FLOAT, max_turning_angle, Wheel, PROPERTY_HINT_RANGE, "0, 90, suffix:°");
}


void Wheel::_ready()
{
    Part::_ready();

    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    if (!get_attachment())
    {
        return;
    }

    ASSIGN_NODE(_wheel_mesh, MeshInstance3D, _wheel_mesh_path);
    ASSIGN_NODE(_force_mesh, MeshInstance3D, _force_mesh_path);

    _rayQueryParameters = PhysicsRayQueryParameters3D::create(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
    _rayQueryParameters->set_exclude(TypedArray<RID>::make(get_attachment()->get_rid() ));
}


void Wheel::_physics_process(double delta)
{
    // Don't tick in engine, ever!!
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    RigidBody3D *attachment = get_attachment();
    if (!attachment)
    {
        return;
    }

    _velocity = (get_global_position() - _previous_position) / delta;
    _previous_position = get_global_position();

    Vector3 forces = get_total_forces(delta);

    if (_force_mesh)
    {
        _force_mesh->set_global_position(_wheel_mesh->get_global_position() + forces * 0.01f);
    }
    // Jolt physics seems to apply forces relative to the center of mass, so we calculate it here
    const Transform3D attachmentTransform = get_attachment()->get_global_transform();
    const Basis attachmentBasis = attachmentTransform.get_basis();
    const Vector3 attachmentPosition = get_attachment()->get_global_position();
    Vector3 attachmentCenterOfMass = attachmentPosition + attachmentBasis.get_quaternion().xform(get_attachment()->get_center_of_mass());
    attachment->apply_force(forces, get_global_position() - attachmentCenterOfMass);

    // Update the rotation of the mesh
    const float velocityAlongZ = attachmentBasis.get_column(2).dot(_velocity);
    _rotational_velocity = velocityAlongZ / _radius;

    Quaternion wheelRotation = _wheel_mesh->get_rotation();
    wheelRotation *= Quaternion(Vector3(0.0f, _rotational_velocity * delta, 0.0f));
    _wheel_mesh->set_quaternion(wheelRotation);
}


void Wheel::power(float amount)
{
    _power = Math::clamp(amount, -1.0f, 1.0f);
}


void Wheel::steer(float amount)
{
    // @todo: implement smoothing?
    Vector3 wheelRotationDegrees = get_rotation_degrees();
    wheelRotationDegrees.y = _max_turning_angle * amount;
    set_rotation_degrees(wheelRotationDegrees);
}


Vector3 Wheel::get_friction_force(double delta, float normal_force) const
{
    const Vector3 xAxis = get_global_transform().basis.get_column(0);

    const float velocityAlongDirection = xAxis.dot(_velocity);
    const float restitutionForce = velocityAlongDirection * normal_force; // How much force it would take to bring the object to a stop in one frame

    const float frictionMagnitude = normal_force * _friction_coeffecient;

    // Get the one with the smallest magnitude -- frictionMagnitude is always positive
    const float finalFrictionMagnitude = Math::min(frictionMagnitude, Math::abs(restitutionForce));

    const Vector3 frictionForce = finalFrictionMagnitude * Math::sign(restitutionForce) * -xAxis;
    return frictionForce;
}


float Wheel::get_applied_torque_force_magnitude() const
{
    // apply torque along the local z axis
    // use get_column to get the mat3 transform column
    const float magnitude = _torque / _radius;
    return magnitude * _power;
}


float Wheel::get_suspension_force_magnitude(float suspension_compression) const
{
    // Don't apply force if the suspension is fully extended
    if (suspension_compression <= 0.0f)
    {
        return 0.0f;
    }
    
    const float suspensionForceMagnitude = suspension_compression * _spring_constant;
    const float dampingForceMagnitude = get_damping_force();

    return suspensionForceMagnitude + dampingForceMagnitude;
}


Vector3 Wheel::get_total_forces(double delta)
{
    bool touchingGround = false;
    float distanceToGround = get_suspension_compression(touchingGround);
    if (!touchingGround)
    {
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    _suspension_velocity = (_previous_distance_to_ground - distanceToGround) / delta;
    _previous_distance_to_ground = distanceToGround;

    Basis basis = get_global_transform().basis;

    const float torqueForceMagnitude = get_applied_torque_force_magnitude();
    const Vector3 torqueForce = torqueForceMagnitude * basis.get_column(2);

    const float suspensionForceMagnitude = get_suspension_force_magnitude(distanceToGround);
    const Vector3 suspensionForce = suspensionForceMagnitude * basis.get_column(1);

    const Vector3 frictionForce = get_friction_force(delta, suspensionForceMagnitude);

    return torqueForce + suspensionForce + frictionForce;
}


float Wheel::get_suspension_compression(bool &touching_ground_out)
{
    PhysicsDirectSpaceState3D *spaceState = get_world_3d()->get_direct_space_state();
    const Vector3 wheelPosition = get_global_position();
    const Vector3 yAxis = get_global_transform().get_basis().get_column(1);

    // Assume the suspension is fully extended by default.
    // Start at the bottom of the wheel, if it's pushed all the way into the suspension
    const Vector3 startPosition = wheelPosition + ((_max_suspension_length - _radius) * yAxis);

    // End at the bottom of the wheel at the origin
    const Vector3 endPosition = wheelPosition + (-_radius * yAxis);

    _rayQueryParameters->set_from(startPosition);
    _rayQueryParameters->set_to(endPosition);
    
    Dictionary result = spaceState->intersect_ray(_rayQueryParameters);

    if (result.is_empty())
    {
        touching_ground_out = false;
        _wheel_mesh->set_position(Vector3(0.0f, 0.f, 0.0f));
        return 0.0f;
    }

    Vector3 floorPosition = result["position"];
    _floor_normal = result["normal"];

    const float distance = (floorPosition - wheelPosition).dot(yAxis) + _radius;

    Vector3 wheelMeshPosition = _wheel_mesh->get_position();
    wheelMeshPosition.y = distance;
    _wheel_mesh->set_position(wheelMeshPosition);

    // UtilityFunctions::print( String("[Wheel] Suspension is {0}m from original position at {1}m").format(Array::make(distance, wheelPosition.y)) );
    touching_ground_out = true;
    return distance;
}


float Wheel::get_damping_force() const
{
    return _suspension_velocity * _damping * get_attachment()->get_mass() * -1.0f;
}
