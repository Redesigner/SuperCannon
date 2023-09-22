#include "cannon_spawner_projectile.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

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
    // UtilityFunctions::print( String("[CannonSpawner] collided with object {0} at position {1} with normal {2}.").format(Array::make(other_body->get_name(), position, normal)) );
    Node3D *projectile = Object::cast_to<Node3D>(_cannon_scene->instantiate());
    get_tree()->get_current_scene()->add_child(projectile);
    projectile->set_global_position(position);
    projectile->look_at(position + normal);

    queue_free();
}
