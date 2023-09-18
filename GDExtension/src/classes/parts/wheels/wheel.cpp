#include "wheel.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/world3d.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Wheel::Wheel()
{
    _friction_coeffecient = 0.0f;
    _radius = 0.0f;

    _max_suspension_length = 0.0f;
    _spring_constant = 0.0f;

    _torque = 0.0f;
}


Wheel::~Wheel()
{
}


void Wheel::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, wheel_mesh_path, Wheel);

    BIND_PROPERTY_HINT(Variant::FLOAT, friction_coeffecient, Wheel, PROPERTY_HINT_RANGE, "0,1,0.1");
    BIND_PROPERTY_HINT(Variant::FLOAT, radius, Wheel, PROPERTY_HINT_NONE, "suffix:m");

    BIND_PROPERTY_HINT(Variant::FLOAT, max_suspension_length, Wheel, PROPERTY_HINT_NONE, "suffix:m");
    BIND_PROPERTY_HINT(Variant::FLOAT, spring_constant, Wheel, PROPERTY_HINT_NONE, "suffix:N/m");

    BIND_PROPERTY_HINT(Variant::FLOAT, torque, Wheel, PROPERTY_HINT_RANGE, "0, 100000, 1, suffix:Nm");
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

    attachment->apply_force(get_total_forces(), get_position());
}


Vector3 Wheel::get_friction_force() const
{
    const Vector3 xAxis = get_global_transform().basis.get_column(0);
    const float velocityAlongSlideDirection = xAxis.dot(get_attachment()->get_linear_velocity());

    // for now, friction is simply based on mass
    const float frictionMagnitude = 9.8 * get_attachment()->get_mass() * _friction_coeffecient;
    
    return Math::min(frictionMagnitude, -velocityAlongSlideDirection) * xAxis;
}


Vector3 Wheel::get_applied_torque_force() const
{
    // apply torque along the local z axis
    // use get_column to get the mat3 transform column
    const float magnitude = _torque / _radius;
    return get_global_transform().basis.get_column(2) * magnitude;
}


Vector3 Wheel::get_suspension_force(float suspension_compression) const
{
    // Don't apply force if the suspension is fully extended
    if (suspension_compression <= 0.0f)
    {
        return Vector3(0.0f, 0.0f, 0.0f);
    }
    
    // it should be negative here
    const float delta = suspension_compression;
    const float magnitude = (delta / _max_suspension_length) * _spring_constant;

    UtilityFunctions::print( String("[Wheel] Spring is {0}m from rest position. Suspension force is {1} N").format(Array::make(delta, magnitude)) );
    return get_global_transform().basis.get_column(1) * magnitude;
}


Vector3 Wheel::get_total_forces() const
{
    bool touchingGround = false;
    float distanceToGround = get_suspension_compression(touchingGround);

    if (!touchingGround)
    {
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    Basis basis = get_global_transform().basis;

    const Vector3 torqueForce = get_applied_torque_force();

    const Vector3 suspensionForce = get_suspension_force(distanceToGround);

    const Vector3 frictionForce = get_friction_force();

    return torqueForce + suspensionForce /*+ frictionForce*/;
}


float Wheel::get_suspension_compression(bool &touching_ground_out) const
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
        return -1.0f;
    }

    Vector3 floorPosition = result["position"];

    const float distance = (floorPosition - wheelPosition).dot(yAxis) + _radius;

    Vector3 wheelMeshPosition = _wheel_mesh->get_position();
    wheelMeshPosition.y = distance;
    _wheel_mesh->set_position(wheelMeshPosition);

    UtilityFunctions::print( String("[Wheel] Suspension is {0}m from original position at {1}m").format(Array::make(distance, wheelPosition.y)) );
    touching_ground_out = true;
    return distance;
}