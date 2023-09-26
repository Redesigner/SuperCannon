#include "controller.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "../body/cannon_body.h"
#include "../cameras/follow_camera3d.h"

using namespace godot;

Controller::Controller()
{
    _control_stack = std::stack<CannonBody *>();

    _camera_rotation_rate = 0.01f;
    _camera_rotation_rate_joystick = 5.0f;
}


Controller::~Controller()
{
}


void Controller::_bind_methods()
{
    BIND_PROPERTY_HINT(Variant::FLOAT, camera_rotation_rate, Controller, PROPERTY_HINT_RANGE, "0, 0.5, suffix:rad/px");

    BIND_PROPERTY_HINT(Variant::FLOAT, camera_rotation_rate_joystick, Controller, PROPERTY_HINT_RANGE, "0, 30, suffix:rad/s");

    BIND_PROPERTY(Variant::NODE_PATH, cannon_body_path, Controller);
    BIND_PROPERTY(Variant::NODE_PATH, follow_camera_path, Controller);
}


void Controller::_ready()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);

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

    if (event->is_action_pressed("exit_control"))
    {
        if (_control_stack.size() <= 1)
        {
            return;
        }

        CannonBody *oldTarget =_control_stack.top();
        _control_stack.pop();
        _camera->set_target(_control_stack.top());
        oldTarget->queue_free();
        return;
    }

    if (event->is_class("InputEventMouseMotion"))
    {
        InputEventMouseMotion *mouseMotionEvent = static_cast<InputEventMouseMotion *>(*event);
        rotate_camera(mouseMotionEvent->get_relative());
        return;
    }
}


void Controller::_physics_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    update_local_inputs(delta);
}


void Controller::take_control(CannonBody *pawn)
{
    if (!pawn)
    {
        return;
    }
    // UtilityFunctions::print( String("[Controller] taking control of new pawn {0}").format(Array::make(pawn->get_name())));

    if (!_control_stack.empty())
    {
        // Make sure our old pawn's inputs are zeroed out
        CannonBody *body = _control_stack.top();
        body->power(0.0f);
        body->steer(0.0f);
        body->control(Vector2());
    }
    _control_stack.push(pawn);
    pawn->take_control(this);
    _camera->set_target(pawn);
}


void Controller::update_local_inputs(double delta)
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

    Vector2 cameraJoystickInput = input->get_vector("camera_left", "camera_right", "camera_down", "camera_up");
    cameraJoystickInput.x *= -1.0f;
    cameraJoystickInput *= _camera_rotation_rate_joystick * delta;
    _camera->rotate(cameraJoystickInput);

    cannonBody->power(-input->get_axis("brake", "throttle"));
    cannonBody->steer(-input->get_axis("turn_left", "turn_right"));
    cannonBody->control(input->get_vector("control_right", "control_left", "control_down", "control_up"));
}

void Controller::rotate_camera(Vector2 input)
{
    if (!_camera)
    {
        return;
    }

    _camera->rotate(input * -_camera_rotation_rate);
}
