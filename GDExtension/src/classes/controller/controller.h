#pragma once

#include <stack>

#include <godot_cpp/classes/node.hpp>

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

private:
    std::stack<CannonBody *> _control_stack;
};
}