#include "slider_ui.h"

using namespace godot;

SliderUI::SliderUI()
{
}

SliderUI::~SliderUI()
{
}

void SliderUI::_bind_methods()
{
    BIND_PROPERTY(Variant::COLOR, button_color, SliderUI);
}

void SliderUI::_notification(int p_what)
{
    switch(p_what)
    {
        case NOTIFICATION_DRAW:
        {
            draw();
        }
        break;
    }
}

void SliderUI::set_button_color(Color color)
{
    if (_button_color == color)
    {
        return;
    }
    _button_color = color;
    queue_redraw();
}

Color godot::SliderUI::get_button_color() const
{
    return _button_color;
}

void SliderUI::draw()
{
    Rect2 region;

    region.set_size(Vector2(50.0f, 50.0f));

    if (region.has_area())
    {
        draw_rect(region, _button_color);
    }
}
