#include "property_editor_ui.h"

#include <godot_cpp/classes/slider.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

PropertyEditorUI::PropertyEditorUI()
{
    property_getter = nullptr;
    property_setter = nullptr;
}

PropertyEditorUI::~PropertyEditorUI()
{
}

void PropertyEditorUI::_bind_methods()
{
    BIND_PROPERTY(Variant::NODE_PATH, slider_path, PropertyEditorUI);
    BIND_PROPERTY(Variant::NODE_PATH, label_path, PropertyEditorUI);
    BIND_PROPERTY(Variant::NODE_PATH, name_label_path, PropertyEditorUI);

    BIND_PROPERTY(Variant::STRING, property_display_name, PropertyEditorUI);

    BIND_PROPERTY(Variant::FLOAT, value_max, PropertyEditorUI);
    BIND_PROPERTY(Variant::FLOAT, value_min, PropertyEditorUI);
    BIND_PROPERTY(Variant::STRING, property_suffix, PropertyEditorUI);

    ClassDB::bind_method(D_METHOD("slider_grabbed"), &slider_grabbed);
    ClassDB::bind_method(D_METHOD("slider_released"), &slider_released);
}

void PropertyEditorUI::_ready()
{
    if (IN_EDITOR())
    {
        return;
    }

    ASSIGN_NODE(slider, Slider, _slider_path);
    ASSIGN_NODE(label, Label, _label_path);
    ASSIGN_NODE(name_label, Label, _name_label_path);

    slider->connect("drag_started", Callable(this, "slider_grabbed"));
    slider->connect("drag_ended", Callable(this, "slider_released"));
}

void PropertyEditorUI::_process(double delta)
{
    if (is_slider_grabbed)
    {
        update_values();
    }
}

void PropertyEditorUI::attach_property(PropertyGetter getter, PropertySetter setter)
{
    property_getter = getter;
    property_setter = setter;

    name_label->set_text(_property_display_name);
    float value = property_getter();
    label->set_text(String("{0} {1}").format(Array::make(value, _property_suffix)));
    slider->set_value((value - _value_min) / (_value_max - _value_min) * 100.0f);
}

void PropertyEditorUI::slider_grabbed()
{
    is_slider_grabbed = true;
}

void PropertyEditorUI::slider_released(bool value_changed)
{
    is_slider_grabbed = false;
}

void PropertyEditorUI::update_values()
{
    if (!slider)
    {
        return;
    }
    if (!property_setter)
    {
        return;
    }
    const float slider_value = slider->get_value() / 100.0f;
    float value = _value_min + (_value_max - _value_min) * slider_value;
    property_setter(value);

    label->set_text(String("{0} {1}").format(Array::make(value, _property_suffix)));
}
