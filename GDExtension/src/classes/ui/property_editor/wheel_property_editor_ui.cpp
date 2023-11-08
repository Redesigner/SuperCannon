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
    BIND_PROPERTY(Variant::NODE_PATH, spring_editor_path, WheelPropertyEditorUI);
    BIND_PROPERTY(Variant::NODE_PATH, torque_editor_path, WheelPropertyEditorUI);
    BIND_PROPERTY(Variant::NODE_PATH, suspension_editor_path, WheelPropertyEditorUI);
}

void WheelPropertyEditorUI::_ready()
{
    if (IN_EDITOR())
    {
        return;
    }

    // ASSIGN_NODE(cannon_body, CannonBody, _cannon_body_path);
    cannon_body = get_node<CannonBody>("/root/TestLevel/Controller/CannonBody");
    wheels = cannon_body->get_parts_of_type<Wheel>();

    ASSIGN_NODE(friction_editor, PropertyEditorUI, _friction_editor_path);
    ASSIGN_NODE(spring_editor, PropertyEditorUI, _spring_editor_path);
    ASSIGN_NODE(torque_editor, PropertyEditorUI, _torque_editor_path);
    ASSIGN_NODE(suspension_editor, PropertyEditorUI, _suspension_editor_path);

    friction_editor->attach_property(
        [this]() { return get_wheel_friction(); },
        [this](float friction){ set_wheel_friction(friction); }
    );
    spring_editor->attach_property(
        [this]() { return get_spring_constant(); },
        [this](float spring){ set_spring_constant(spring); }
    );
    torque_editor->attach_property(
        [this]() { return get_wheel_torque(); },
        [this](float torque){ set_wheel_torque(torque); }
    );
    suspension_editor->attach_property(
        [this]() { return get_suspension(); },
        [this](float suspension){ set_suspension(suspension); }
    );

    if (!cannon_body)
    {
        return;
    }    
}

void WheelPropertyEditorUI::set_wheel_friction(float friction)
{
    for (Wheel *wheel : wheels)
    {
        wheel->set_friction_coeffecient(friction);
    }
}

float WheelPropertyEditorUI::get_wheel_friction() const
{
    if (wheels.size() <= 0)
    {
        return 0.0f;
    }
    return wheels[0]->get_friction_coeffecient();
}

void WheelPropertyEditorUI::set_spring_constant(float spring)
{
    for (Wheel *wheel : wheels)
    {
        wheel->set_spring_constant(spring);
    }
}

float WheelPropertyEditorUI::get_spring_constant() const
{
    if (wheels.size() <= 0)
    {
        return 0.0f;
    }
    return wheels[0]->get_spring_constant();
}

void WheelPropertyEditorUI::set_wheel_torque(float torque)
{
    for (Wheel *wheel : wheels)
    {
        wheel->set_torque(torque);
    }
}

float WheelPropertyEditorUI::get_wheel_torque() const
{
    if (wheels.size() <= 0)
    {
        return 0.0f;
    }
    return wheels[0]->get_torque();
}

void WheelPropertyEditorUI::set_suspension(float suspension)
{
    for (Wheel *wheel : wheels)
    {
        wheel->set_max_suspension_length(suspension);
        Vector3 position = wheel->get_position();
        position.y = 1 - suspension;
        wheel->set_position(position);
    }
}

float WheelPropertyEditorUI::get_suspension() const
{
    if (wheels.size() <= 0)
    {
        return 0.0f;
    }
    return wheels[0]->get_max_suspension_length();
}