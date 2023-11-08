#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

#include "../classes/body/cannon_body.h"

#include "../classes/cameras/follow_camera3d.h"

#include "../classes/controller/controller.h"

#include "../classes/parts/part.h"
#include "../classes/parts/wheels/wheel.h"
#include "../classes/parts/cannons/cannon.h"

#include "../classes/projectiles/projectile.h"
#include "../classes/projectiles/cannon_spawner_projectile.h"

#include "../classes/ui/slider/slider_ui.h"
#include "../classes/ui/property_editor/property_editor_ui.h"
#include "../classes/ui/property_editor/wheel_property_editor_ui.h"

using namespace godot;

void initialize_transhuman_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ClassDB::register_class<CannonBody>();

    ClassDB::register_class<FollowCamera3D>();

    ClassDB::register_class<Controller>();

    ClassDB::register_class<Part>();
    ClassDB::register_class<Wheel>();
    ClassDB::register_class<Cannon>();

    ClassDB::register_class<Projectile>();
    ClassDB::register_class<CannonSpawnerProjectile>();

    ClassDB::register_class<SliderUI>();
    ClassDB::register_class<PropertyEditorUI>();
    ClassDB::register_class<WheelPropertyEditorUI>();
}

void uninitialize_transhuman_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT transhuman_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_transhuman_module);
    init_obj.register_terminator(uninitialize_transhuman_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}