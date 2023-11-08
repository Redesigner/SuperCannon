#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../../macros/class_macros.h"

#include <vector>

namespace godot
{
class Wheel;
class CannonBody;
class PropertyEditorUI;
class WheelPropertyEditorUI : public Control
{
    GDCLASS(WheelPropertyEditorUI, Control);

public:
    WheelPropertyEditorUI();
    ~WheelPropertyEditorUI();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    void set_wheel_friction(float friction);
    float get_wheel_friction() const;

    void set_spring_constant(float spring);
    float get_spring_constant() const;

    void set_wheel_torque(float torque);
    float get_wheel_torque() const;

    void set_suspension(float suspension);
    float get_suspension() const;

private:
    std::vector<Wheel *> wheels;

    DECLARE_PROPERTY(NodePath, cannon_body_path);
    CannonBody *cannon_body;

    DECLARE_PROPERTY(NodePath, friction_editor_path);
    PropertyEditorUI *friction_editor;

    DECLARE_PROPERTY(NodePath, spring_editor_path);
    PropertyEditorUI *spring_editor;

    DECLARE_PROPERTY(NodePath, torque_editor_path);
    PropertyEditorUI *torque_editor;

    DECLARE_PROPERTY(NodePath, suspension_editor_path);
    PropertyEditorUI *suspension_editor;
};
}