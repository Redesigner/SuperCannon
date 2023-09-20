#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

#include "../classes/cameras/follow_camera3d.h"

#include "../classes/parts/part.h"
#include "../classes/parts/wheels/wheel.h"
#include "../classes/parts/cannons/cannon.h"

using namespace godot;

void initialize_transhuman_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ClassDB::register_class<FollowCamera3D>();

    ClassDB::register_class<Part>();
    ClassDB::register_class<Wheel>();
    ClassDB::register_class<Cannon>();
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