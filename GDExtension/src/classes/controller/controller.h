#pragma once

#include <godot_cpp/classes/node.hpp>

#include <stack>

#include "../../macros/class_macros.h"

namespace godot
{
class CannonBody;
class Controller : public Node
{
    GDCLASS(Controller, Node);

public:
    Controller();
    ~Controller();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    virtual void _input(const Ref<InputEvent> &event) override;

    virtual void _physics_process(double delta) override;

private:
    void update_local_inputs();

    std::stack<CannonBody *> _control_stack;

};
}