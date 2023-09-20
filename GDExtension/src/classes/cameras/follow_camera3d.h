#pragma once

#include <godot_cpp/classes/camera3d.hpp>

#include "../../macros/class_macros.h"

namespace godot
{
class FollowCamera3D : public Camera3D
{
    GDCLASS(FollowCamera3D, Camera3D);

public:
    FollowCamera3D();
    ~FollowCamera3D();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    virtual void _process(double delta) override;

    void set_rotation_offset(Vector3 rotation_offset);
    Vector3 get_rotation_offset() const;

private:
    Node3D *_target;
    DECLARE_PROPERTY(NodePath, target_path);

    DECLARE_PROPERTY(bool, track_position);
    DECLARE_PROPERTY(Vector3, position_offset);

    DECLARE_PROPERTY(bool, track_rotation);
    Quaternion _rotation_offset;
    DECLARE_PROPERTY(float, rotation_tracking_rate);

    bool _in_editor;
};
}