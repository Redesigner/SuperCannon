#include "cannon_body.h"

#include "../parts/part.h"

using namespace godot;

CannonBody::CannonBody()
{
    _attached_parts = std::vector<Part *>();
}

CannonBody::~CannonBody()
{
}

void CannonBody::_bind_methods()
{
}

void CannonBody::_ready()
{
    TypedArray<Node> children = get_children();
    for (int i = 0; i < children.size(); i++)
    {
        if (Part *part = Object::cast_to<Part>(children[i]))
        {
            _attached_parts.push_back(part);
        }
    }
}


void CannonBody::power(float input)
{
    for (Part *part : _attached_parts)
    {
        part->power(input);
    }
}


void CannonBody::steer(float input)
{
    for (Part *part : _attached_parts)
    {
        part->steer(input);
    }
}


void CannonBody::control(Vector2 input)
{
    for (Part *part : _attached_parts)
    {
        part->control(input);
    }
}


void CannonBody::activate()
{
    for (Part *part : _attached_parts)
    {
        part->activate();
    }
}


void CannonBody::_notification(int p_what)
{
}
