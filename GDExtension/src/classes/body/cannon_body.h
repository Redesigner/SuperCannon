#pragma once

#include <godot_cpp/classes/rigid_body3d.hpp>

#include <vector>

namespace godot
{
class Part;
class Controller;
class CannonBody : public RigidBody3D
{
    GDCLASS(CannonBody, RigidBody3D);

public:
    CannonBody();
    ~CannonBody();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    virtual void _physics_process(double delta) override;

    void take_control(Controller *controller);

    void power(float input);

    void steer(float input);

    void control(Vector2 input);

    void activate();

    Controller * get_controller() const;

    template<typename T>
    std::vector<T *> get_parts_of_type() const
    {
        std::vector<T *> returned_parts = std::vector<T *>();
        for (Part *part : attached_parts)
        {
            if (T* typed_part = Object::cast_to<T>(part))
            {
                returned_parts.push_back(typed_part);
            }
        }
        return returned_parts;
    }

protected:
    void _notification(int p_what);

private:
    // use ref counted instead of pointer?
    std::vector<Part *> attached_parts;

    Transform3D spawn_transform;

    Controller *controller = nullptr;
};
}