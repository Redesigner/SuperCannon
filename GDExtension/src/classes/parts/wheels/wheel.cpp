#include "wheel.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/world3d.hpp>

using namespace godot;

Wheel::Wheel()
{
    _is_grounded = false;
}

Wheel::~Wheel()
{
}

void Wheel::_bind_methods()
{
    BIND_PROPERTY_HINT(Variant::FLOAT, frictionCoeffecient, Wheel, PROPERTY_HINT_RANGE, "0,1,0.1");
}

void Wheel::_ready()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

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

}

float Wheel::get_friction_force() const
{
    if (!get_attachment())
    {
        return 0.0f;
    }
    if (!_is_grounded)
    {
        return 0.0f;
    }

    
}

float Wheel::get_distance_to_ground() const
{
    PhysicsDirectSpaceState3D *spaceState = get_world_3d()->get_direct_space_state();
    const Vector3 startPosition = get_global_position();
    const Vector3 endPosition = startPosition - Vector3(0.0f, _radius + _maxSuspensionLength, 0.0f);
    _rayQueryParameters->set_from(startPosition);
    _rayQueryParameters->set_to(endPosition);
    
    Dictionary result = spaceState->intersect_ray(_rayQueryParameters);

    if (result.is_empty())
    {
        return -1.0f;
    }
    
    Vector3 floorPosition = result["position"];
    return startPosition.z - startPosition.z;
}