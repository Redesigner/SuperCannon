#include "cannon.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../../projectiles/projectile.h"
#include "../../body/cannon_body.h"

using namespace godot;

Cannon::Cannon()
{
    _pitch_speed = 0.0f;
    _min_pitch = 0.0f;
    _max_pitch = 90.0f;

    _yaw_speed = 0.0f;

    _control_input = Vector2();
}

Cannon::~Cannon()
{
}

void Cannon::_bind_methods()
{
    BIND_PROPERTY_HINT(Variant::FLOAT, pitch_speed, Cannon, PROPERTY_HINT_RANGE, "0, 720, suffix:deg/s");
    BIND_PROPERTY_HINT(Variant::FLOAT, max_pitch, Cannon, PROPERTY_HINT_RANGE, "-90, 90, suffix:deg");
    BIND_PROPERTY_HINT(Variant::FLOAT, min_pitch, Cannon, PROPERTY_HINT_RANGE, "-90, 90, suffix:deg");

    BIND_PROPERTY_HINT(Variant::FLOAT, yaw_speed, Cannon, PROPERTY_HINT_RANGE, "0, 720, suffix:deg/s");

    BIND_PROPERTY_HINT(Variant::FLOAT, projectile_velocity, Cannon, PROPERTY_HINT_RANGE, "0, 10000, suffix:m/s");

    BIND_PROPERTY(Variant::NODE_PATH, barrel_path, Cannon);
    BIND_PROPERTY(Variant::NODE_PATH, base_path, Cannon);

    BIND_PROPERTY_HINT(Variant::STRING, projectile_scene_path, Cannon, PROPERTY_HINT_FILE, "");
}


void Cannon::_ready()
{
    Part::_ready();

    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    _projectile_scene = ResourceLoader::get_singleton()->load(_projectile_scene_path);

    ASSIGN_NODE(_barrel, Node3D, _barrel_path);
    ASSIGN_NODE(_base, Node3D, _base_path);
}


void Cannon::_physics_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    if (!_base || !_barrel)
    {
        return;
    }

    Vector3 barrelRotation = _barrel->get_rotation_degrees();
    barrelRotation.x += _control_input.y * _pitch_speed * delta;
    barrelRotation.x = Math::clamp(barrelRotation.x, _min_pitch, _max_pitch);
    _barrel->set_rotation_degrees(barrelRotation);

    Vector3 baseRotation = _base->get_rotation_degrees();
    baseRotation.y += _control_input.x * _yaw_speed * delta;
    _base->set_rotation_degrees(baseRotation);
}

void Cannon::control(Vector2 input)
{
    _control_input = input;
}

void Cannon::activate()
{
    Projectile *projectile = Object::cast_to<Projectile>(_projectile_scene->instantiate());
    const Vector3 projectileFireDirection = -_barrel->get_global_transform().get_basis().get_column(2);

    get_tree()->get_current_scene()->add_child(projectile);
    Transform3D projectileTransform = _barrel->get_global_transform();
    projectile->set_global_transform(projectileTransform.translated(projectileFireDirection * 1.0f));

    projectile->set_linear_velocity(projectileFireDirection * _projectile_velocity + get_attachment()->get_linear_velocity());

    const float projectileMomentum = _projectile_velocity * projectile->get_mass();
    Vector3 attachmentCenterOfMass = get_attachment()->get_global_position() + get_attachment()->get_quaternion().xform(get_attachment()->get_center_of_mass());
    get_attachment()->apply_impulse(projectileMomentum / get_attachment()->get_mass() * -projectileFireDirection, get_global_position() - attachmentCenterOfMass);

    // UtilityFunctions::print( String("[Projectile] projectile spawned, owner set to {0}").format(Array::make(get_attachment()->get_name())) );
    projectile->set_cannon_owner(get_attachment());
}
