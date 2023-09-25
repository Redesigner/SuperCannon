#include "controller.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../body/cannon_body.h"
#include "../cameras/follow_camera3d.h"

using namespace godot;

Controller::Controller()
{
    _control_stack = std::stack<CannonBody *>();
}


Controller::~Controller()
{
}


void Controller::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, cannon_body_path, Controller);
    BIND_PROPERTY(Variant::NODE_PATH, follow_camera_path, Controller);
}


void Controller::_ready()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    CannonBody *body = Object::cast_to<CannonBody>(get_node_or_null(_cannon_body_path));
    if (!body)
    {
        WARN_PRINT("Cannon Body found at path not valid.");
        return;
    }
    ASSIGN_NODE(_camera, FollowCamera3D, _follow_camera_path);

    take_control(body);
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
        return;
    }

    if (event->is_action_pressed("ui_cancel"))
    {
        get_tree()->quit();
        return;
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


void Controller::take_control(CannonBody *pawn)
{
    if (!pawn)
    {
        return;
    }
    UtilityFunctions::print( String("[Controller] taking control of new pawn {0}").format(Array::make(pawn->get_name())));
    _control_stack.push(pawn);
    pawn->take_control(this);
    _camera->set_target(pawn);
}


void Controller::update_local_inputs()
{
    if (_control_stack.empty())
    {
        return;
    }
    CannonBody *cannonBody = _control_stack.top();

    if (!cannonBody)
    {
        _control_stack.pop();
        WARN_PRINT("Controller has a cannon set, but it is null. Did the cannon get deleted?");
        return;
    }
    Input *input = Input::get_singleton();

    cannonBody->power(-input->get_axis("brake", "throttle"));
    cannonBody->steer(-input->get_axis("turn_left", "turn_right"));
    cannonBody->control(input->get_vector("control_right", "control_left", "control_down", "control_up"));
}
