#include "projectile.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/physics_server3d.hpp>
#include <godot_cpp/classes/physics_direct_body_state3d.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Projectile::Projectile()
{
    // The on_collision method doesn't work here unless contact monitoring is enabled
    // and the max contacts reported is at least one
    // Also consider enabling CCD
    set_contact_monitor(true);
    set_max_contacts_reported(1);
}

Projectile::~Projectile()
{
}

void Projectile::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("collide", "body"), &Projectile::collide);
}

void Projectile::_ready()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    connect("body_shape_entered", Callable(this, "collide"));
}

void Projectile::_integrate_forces(PhysicsDirectBodyState3D *state)
{
}

// I'd REALLY like to do this another way, but it seems without making it a module, this is the best I can do
void Projectile::collide(RID body_rid, Node *body, int body_shape_index, int local_shape_index)
{

    Node3D *otherBody = Object::cast_to<Node3D>(body);
    const PhysicsDirectBodyState3D *state = PhysicsServer3D::get_singleton()->body_get_direct_state(get_rid());

    if (!otherBody)
    {
        WARN_PRINT("Collided with invalid Body.");
        return;
    }
    if (!state)
    {
        WARN_PRINT(String("Unable to get direct body state for RigidBody3d with RID '{0}'.").format(Array::make(get_rid())) );
        return;
    }
    if (state->get_contact_count() <= 0)
    {
        return;
    }
    const Vector3 globalPosition = state->get_contact_collider_position(local_shape_index);
    const Vector3 normal = state->get_contact_local_normal(local_shape_index);

    UtilityFunctions::print( String("[Projectile] collision occurred at {0} with normal {1}").format(Array::make(globalPosition, normal)) );

    on_collision(otherBody, globalPosition, normal);
}

void Projectile::on_collision(Node3D *other_body, Vector3 position, Vector3 normal)
{
}

void Projectile::set_cannon_owner(CannonBody *owner)
{
    _cannon_owner = owner;
}

CannonBody *Projectile::get_cannon_owner() const
{
    return _cannon_owner;
}
