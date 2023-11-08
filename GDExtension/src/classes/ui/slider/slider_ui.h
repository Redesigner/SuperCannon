#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../../macros/class_macros.h"

namespace godot
{
class SliderUI: public Control
{
    GDCLASS(SliderUI, Control);

public:
    SliderUI();
    ~SliderUI();

protected:
    static void _bind_methods();

    void _notification(int p_what);

public:
    void set_button_color(Color color);
    Color get_button_color() const;

private:
    void draw();

    
    Color _button_color;
};
}