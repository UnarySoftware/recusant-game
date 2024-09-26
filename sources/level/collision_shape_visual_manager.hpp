#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/compressed_texture2d.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

#include <shared/util.hpp>

#define DEFINE_VARIABLE(name) \
godot::Ref<godot::StandardMaterial3D> _##name; 

#define DEFINE_FUNCTION(name) \
void set_##name(const godot::Ref<godot::StandardMaterial3D> p_##name) { \
_##name = p_##name; } \
godot::Ref<godot::StandardMaterial3D> get_##name() const { \
return _##name; }

#define _BIND_PROPERTY(name, type) \
godot::ClassDB::bind_method(godot::D_METHOD(godot::String(__CONCAT(get_, name))), &CollisionShapeVisualManager::get_##name); \
godot::ClassDB::bind_method(godot::D_METHOD(godot::String(__CONCAT(set_, name)), godot::String(__CONCAT(p_, name))), &CollisionShapeVisualManager::set_##name); \
ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, #name, godot::PropertyHint::PROPERTY_HINT_RESOURCE_TYPE, "StandardMaterial3D"), godot::String(__CONCAT(set_, name)), godot::String(__CONCAT(get_, name))); \
BIND_ENUM_CONSTANT(type);

namespace recusant
{
    class CollisionShapeVisualManager : public godot::Node
    {
        GD_CLASS(CollisionShapeVisualManager, Node);

    private:

        DEFINE_VARIABLE(zone);
        DEFINE_VARIABLE(clip);
        DEFINE_VARIABLE(trigger);

    public:

        enum Type
        {
            Zone = 0,
            Clip,
            Trigger,
        };

        DEFINE_FUNCTION(zone);
        DEFINE_FUNCTION(clip);
        DEFINE_FUNCTION(trigger);

    protected:

        static void _bind_methods()
        {
            _BIND_PROPERTY(zone, Zone);
            _BIND_PROPERTY(clip, Clip);
            _BIND_PROPERTY(trigger, Trigger);
        }

    };
}

VARIANT_ENUM_CAST(recusant::CollisionShapeVisualManager::Type);


#undef DEFINE_VARIABLE
#undef DEFINE_FUNCTION
#undef _BIND_PROPERTY
