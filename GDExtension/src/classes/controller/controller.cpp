#include "controller.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include "../body/cannon_body.h"

using namespace godot;

Controller::Controller()
{
}


Controller::~Controller()
{
}


void Controller::_bind_methods()
{
}


void Controller::_ready()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
}

void Controller::_input(const Ref<InputEvent> &event)
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    if (_control_stack.empty())
    {
        return;
    }

    if (event->is_action_pressed("activate"))
    {
        _control_stack.top()->activate();
    }
}


void Controller::_physics_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    update_local_inputs();
}

void Controller::update_local_inputs()
{
    if (_control_stack.empty())
    {
        return;
    }

    CannonBody *cannonBody = _control_stack.top();
    Input *input = Input::get_singleton();

    cannonBody->power(input->get_axis("brake", "throttle"));
    cannonBody->steer(input->get_axis("turn_left", "turn_right"));
    cannonBody->control(input->get_vector("control_right", "control_left", "control_down", "control_up"));
}
