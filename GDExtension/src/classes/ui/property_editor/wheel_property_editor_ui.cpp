#include "wheel_property_editor_ui.h"

#include "property_editor_ui.h"
#include "../../body/cannon_body.h"
#include "../../parts/wheels/wheel.h"

#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

WheelPropertyEditorUI::WheelPropertyEditorUI()
{
}

WheelPropertyEditorUI::~WheelPropertyEditorUI()
{
}

void WheelPropertyEditorUI::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, cannon_body_path, WheelPropertyEditorUI);
    BIND_PROPERTY(Variant::NODE_PATH, friction_editor_path, WheelPropertyEditorUI);
}

void WheelPropertyEditorUI::_ready()
{
    if (IN_EDITOR())
    {
        return;
    }

    // ASSIGN_NODE(cannon_body, CannonBody, _cannon_body_path);
    cannon_body = get_node<CannonBody>("/root/TestLevel/Controller/CannonBody");
    ASSIGN_NODE(friction_editor, PropertyEditorUI, _friction_editor_path);
    friction_editor->attach_property(nullptr, [this](float friction){ set_wheel_friction(friction); });

    if (!cannon_body)
    {
        return;
    }
    
    wheels = cannon_body->get_parts_of_type<Wheel>();
}

void WheelPropertyEditorUI::set_wheel_friction(float friction)
{
    for (Wheel *wheel : wheels)
    {
        wheel->set_spring_constant(friction * 1000.0f);
    }
    UtilityFunctions::print(String("value set to {0}").format(Array::make(friction * 1000.0f)) );
}
