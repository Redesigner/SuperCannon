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

    virtual void _process(double delta) override;

    void attach_property(PropertyGetter getter, PropertySetter setter);

private:
    void slider_grabbed();
    void slider_released(bool value_changed);

    void update_values();


    DECLARE_PROPERTY(NodePath, slider_path);
    Slider *slider;

    DECLARE_PROPERTY(NodePath, label_path);
    Label *label;

    DECLARE_PROPERTY(NodePath, name_label_path);
    Label *name_label;
    
    DECLARE_PROPERTY(String, property_display_name);

    DECLARE_PROPERTY(float, value_max);
    DECLARE_PROPERTY(float, value_min);
    DECLARE_PROPERTY(String, property_suffix);

    PropertyGetter property_getter;

    PropertySetter property_setter;

    bool is_slider_grabbed = false;
    
};
}