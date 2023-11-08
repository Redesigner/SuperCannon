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

    ClassDB::bind_method(D_METHOD("set_property_value"), &set_property_value);
}

void PropertyEditorUI::_ready()
{
    if (IN_EDITOR())
    {
        return;
    }

    ASSIGN_NODE(slider, Slider, _slider_path);
    ASSIGN_NODE(label, Label, _label_path);

    slider->connect("drag_ended", Callable(this, "set_property_value"));
}

void PropertyEditorUI::attach_property(PropertyGetter getter, PropertySetter setter)
{
    property_getter = getter;
    property_setter = setter;
}

void PropertyEditorUI::set_property_value(bool value_changed)
{
    if (!value_changed)
    {
        return;
    }
    if (!slider)
    {
        return;
    }
    if (property_setter)
    {
        property_setter(slider->get_value() / 100.0f);
    }
}
