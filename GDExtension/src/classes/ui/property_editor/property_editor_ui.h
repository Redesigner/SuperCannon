#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../../macros/class_macros.h"

#include <functional>

namespace godot
{
class Slider;
class Label;
class PropertyEditorUI : public Control
{
    // typedef void(*PropertySetter)(Object *, float);
    // typedef float(*PropertyGetter)(Object *);
    typedef std::function<void(float)> PropertySetter;
    typedef std::function<float()> PropertyGetter;

    GDCLASS(PropertyEditorUI, Control);

public:
    PropertyEditorUI();
    ~PropertyEditorUI();

protected:
    static void _bind_methods();

public:
    virtual void _ready() override;

    void attach_property(PropertyGetter getter, PropertySetter setter);

private:
    void set_property_value(bool value_changed);


    DECLARE_PROPERTY(NodePath, slider_path);
    Slider *slider;

    DECLARE_PROPERTY(NodePath, label_path);
    Label *label;

    PropertyGetter property_getter;

    PropertySetter property_setter;
    
};
}