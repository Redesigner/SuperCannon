#include "follow_camera3d.h"

#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

FollowCamera3D::FollowCamera3D()
{
    _target = nullptr;
    _target_path = "";

    _track_position = false;
    _position_offset = Vector3(0.0f, 0.0f, 0.0f);

    _track_rotation = false;
    _rotation_offset = Quaternion();
    _rotation_tracking_rate = 0.0f;

    _in_editor = false;
}


FollowCamera3D::~FollowCamera3D()
{
}


void FollowCamera3D::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, target_path, FollowCamera3D);

    BIND_PROPERTY(Variant::BOOL, track_position, FollowCamera3D);
    BIND_PROPERTY(Variant::VECTOR3, position_offset, FollowCamera3D);

    BIND_PROPERTY(Variant::BOOL, track_rotation, FollowCamera3D)
    BIND_PROPERTY_HINT(Variant::VECTOR3, rotation_offset, FollowCamera3D, PROPERTY_HINT_NONE, "suffix:deg"); // can't use special characters from here :'(
    BIND_PROPERTY_HINT(Variant::FLOAT, rotation_tracking_rate, FollowCamera3D, PROPERTY_HINT_RANGE, "0, 1");
}


void FollowCamera3D::_ready()
{
    _in_editor = Engine::get_singleton()->is_editor_hint();

    if (_in_editor)
    {
        return;
    }

    ASSIGN_NODE(_target, Node3D, _target_path);


    Vector3 calculatedOffset = _position_offset;
    const Basis targetBasis = _target->get_global_transform().get_basis();
    if (_track_rotation)
    {
        Transform3D transform = get_global_transform();
        Basis basis = transform.get_basis();
        Quaternion targetQuat = targetBasis.get_quaternion();
        Quaternion newQuat = targetQuat * _rotation_offset;
        basis.set_quaternion(newQuat);
        transform.set_basis(basis);
        set_transform(transform);

        calculatedOffset = targetBasis.get_quaternion().xform(calculatedOffset); 
    }
    if (_track_position)
    {
        set_global_position(_target->get_global_position() + calculatedOffset);
    }
}


void FollowCamera3D::_process(double delta)
{
    if (_in_editor)
    {
        return;
    }

    if (!_target)
    {
        return;
    }

    Vector3 calculatedOffset = _position_offset;
    const Basis targetBasis = _target->get_global_transform().get_basis();

    if (_track_rotation)
    {
        Transform3D transform = get_global_transform();
        Basis basis = transform.get_basis();
        Quaternion targetQuat = targetBasis.get_quaternion();
        Quaternion oldQuat = basis.get_quaternion();
        Quaternion newQuat = targetQuat * _rotation_offset;
        basis.set_quaternion(oldQuat.slerp(newQuat, _rotation_tracking_rate));
        transform.set_basis(basis);
        set_transform(transform);

        calculatedOffset = targetBasis.get_quaternion().xform(calculatedOffset); 
    }

    if (_track_position)
    {
        set_global_position(_target->get_global_position() + calculatedOffset);
    }

}


void FollowCamera3D::set_rotation_offset(Vector3 rotation_offset)
{
    _rotation_offset = Quaternion(rotation_offset * (Math_PI / 180.0f));
}


Vector3 FollowCamera3D::get_rotation_offset() const
{
    return _rotation_offset.get_euler() * (180.0f / Math_PI);
}
