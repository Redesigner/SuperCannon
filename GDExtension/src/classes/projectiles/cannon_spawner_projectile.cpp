#include "cannon_spawner_projectile.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../body/cannon_body.h"
#include "../controller/controller.h"

using namespace godot;

CannonSpawnerProjectile::CannonSpawnerProjectile()
{
}


CannonSpawnerProjectile::~CannonSpawnerProjectile()
{
}


void CannonSpawnerProjectile::_bind_methods()
{
    BIND_PROPERTY_HINT(Variant::STRING, cannon_scene_path, CannonSpawnerProjectile, PROPERTY_HINT_FILE, "");
}


void CannonSpawnerProjectile::_ready()
{
    Projectile::_ready();

    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    _cannon_scene = ResourceLoader::get_singleton()->load(_cannon_scene_path);
}


void CannonSpawnerProjectile::on_collision(Node3D *other_body, Vector3 position, Vector3 normal)
{
    Node *node = _cannon_scene->instantiate();
    if (!node)
    {
        WARN_PRINT("[CannonSpawnerProjectile] Failed to instantiate PackedScene.");
        return;
    }
    CannonBody *projectile = Object::cast_to<CannonBody>(node);
    if (!projectile)
    {
        WARN_PRINT("[CannonSpawnerProjectile] Projectile scene not valid. It should be set to an object of type CannonBody");
        return;
    }
    get_tree()->get_current_scene()->add_child(projectile);
    projectile->set_global_position(position);

    Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
    // Prevent axis locking
    if (Math::abs(normal.y) >= 0.95f)
    {
        up = Vector3(position - get_global_position()).normalized();
    }
    projectile->look_at(position + normal, up);

    CannonBody *body = get_cannon_owner();
    if (!body)
    {
        UtilityFunctions::print("[CannonSpawnerProejctile] Unable to find owner of projectile. Did you forget to set it?");
        return;
    }
    Controller *controller = body->get_controller();
    if (!controller)
    {
        UtilityFunctions::print("[CannonSpawnerProejctile] Unable to find controller. Did you forget to assign it to the CannonBody?");
    }
    controller->take_control(projectile);

    queue_free();
}
