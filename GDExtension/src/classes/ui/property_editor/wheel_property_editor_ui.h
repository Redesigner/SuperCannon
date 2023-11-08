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

private:
    std::vector<Wheel *> wheels;

    DECLARE_PROPERTY(NodePath, cannon_body_path);
    CannonBody *cannon_body;

    DECLARE_PROPERTY(NodePath, friction_editor_path);
    PropertyEditorUI *friction_editor;
};
}