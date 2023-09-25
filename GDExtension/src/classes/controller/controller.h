#pragma once

#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/classes/input_event.hpp>

#include <stack>

#include "../../macros/class_macros.h"

namespace godot
{
class CannonBody;
class InputEvent;

// this has to extend node3d so we can place it in the world properly
class Controller : public Node3D
{
    GDCLASS(Controller, Node3D);

public:
    Controller();
    ~Controller();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    virtual void _input(const Ref<InputEvent> &event) override;

    virtual void _physics_process(double delta) override;

    void take_control(CannonBody *pawn);

private:
    void update_local_inputs();

    DECLARE_PROPERTY(NodePath, cannon_body_path);

    std::stack<CannonBody *> _control_stack;

};
}