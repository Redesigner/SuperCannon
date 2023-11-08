#pragma once

#define WARNING_HELPER(string, ...) UtilityFunctions::push_warning(__FILE__, ":", __LINE__, String(" - " string).format(Array::make(__VA_ARGS__)));

/// @brief Binds the property's getter and setter methods, necessary for using in godot's editor API
/// @param variant Variant type used by the editor. It doesn't have to match the actual type declared in the header, which will not always be an option
/// @param p_name property name, without the underscore
/// @param class current class
/// @param hint_type PROPERTY_HINT type
/// @param hint hint string. These vary greatly depending on what you want to do and what the PROPERTY_HINT type is.
#define BIND_PROPERTY_HINT(variant, p_name, class, hint_type, hint)\
    ClassDB::bind_method(D_METHOD("get_"#p_name), &class::get_ ##p_name);\
    ClassDB::bind_method(D_METHOD("set_"#p_name, "p_"#p_name), &class::set_ ##p_name);\
    ClassDB::add_property( #class, PropertyInfo((variant), #p_name, hint_type, hint), "set_"#p_name, "get_"#p_name);\


/// @brief Binds the property's getter and setter methods, necessary for using in godot's editor API
/// @param variant Variant type used by the editor. It doesn't have to match the actual type declared in the header, which will not always be an option
/// @param p_name property name, without the underscore
/// @param class current class
#define BIND_PROPERTY(variant, p_name, class)\
    ClassDB::bind_method(D_METHOD("get_"#p_name), &class::get_ ##p_name);\
    ClassDB::bind_method(D_METHOD("set_"#p_name, "p_"#p_name), &class::set_ ##p_name);\
    ClassDB::add_property( #class, PropertyInfo((variant), #p_name), "set_"#p_name, "get_"#p_name);\


/// @brief Declares a private property and the appropriate getter and setter functions Sets the default value in the header
/// @n To be used in conjunction with BIND_PROPERTY or BIND_PROPERTY_HINT in _bind_methods()
/// @param type is the type of the property
/// @param p_name is the name of the property - and underscore prefix will be added automatically
#define DECLARE_PROPERTY_DEFAULT(type, p_name, default_value)\
private:\
    type _##p_name = default_value;\
public:\
    void set_##p_name(type value)\
    {\
        _##p_name = value;\
    }\
    type get_##p_name() const\
    {\
        return _##p_name;\
    }\
private:


/// @brief Declares a private property and the appropriate getter and setter functions Sets the default value in the header
/// @n To be used in conjunction with BIND_PROPERTY or BIND_PROPERTY_HINT in _bind_methods()
/// @param type is the type of the property
/// @param p_name is the name of the property - and underscore prefix will be added automatically
#define DECLARE_PROPERTY(type, p_name)\
private:\
    type _##p_name;\
public:\
    void set_##p_name(type value)\
    {\
        _##p_name = value;\
    }\
    type get_##p_name() const\
    {\
        return _##p_name;\
    }\
private:


/// @brief Gets a Node from a NodePath and casts it to the appropriate type.
/// Throws a warning if the node cannot be found, requires access to variant/utility_function.hpp
/// @param node node being assigned to
/// @param path NodePath being looked up
/// @note node should be a pointer type
#define ASSIGN_NODE(node, node_type, path)\
    if (path.is_empty())\
    {\
        WARNING_HELPER("Node path for '"#node"' not set.")\
    }\
    else\
    {\
        node = get_node<node_type>(path);\
        if (!node)\
        {\
            WARNING_HELPER("Could not find node of type '" #node_type"' at path '{0}'.", path)\
        }\
    }\


/// @brief Gets a Node from a NodePath and casts it to the appropriate type
/// @param node node being assigned to
/// @param path NodePath being looked up
#define ASSIGN_NODE_NOWARN(node, node_type, path) node = get_node<node_type>(path);

#define IN_EDITOR() Engine::get_singleton()->is_editor_hint()