#pragma once

#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/templates/hash_map.hpp>
#include <shared/components/component.hpp>

#include <shared/util.hpp>

namespace recusant
{
    class Component;
    class AuthoritySynchronizer;
    class EntitySynchronizer;

    class Entity : public godot::Node3D
    {
        GD_CLASS(Entity, Node3D);

    private:

        bool _is_ready = false;

        // All entities are always owned by server first
        int32_t _authority = SERVER_PEER;

        AuthoritySynchronizer* _authority_synchronizer = nullptr;
        EntitySynchronizer* _entity_synchronizer = nullptr;

        bool _got_components = false;
        godot::HashMap<godot::StringName, Component*> _components;
        void _get_components();

    protected:

        static void _bind_methods();

    public:

        godot::StringName on_authority_change;


        Entity()
        {
            on_authority_change = "on_authority_change";
        }

        AuthoritySynchronizer* get_authority_synchronizer();
        EntitySynchronizer* get_entity_synchronizer();

        Component* get_component(const godot::StringName& p_component_name)
        {
            if (!_got_components)
            {
                _get_components();
            }
            if (!_components.has(p_component_name))
            {
                return nullptr;
            }
            return _components.get(p_component_name);
        }

        template <class T>
        T* get_component()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit Component to be used with get_component()");
            return cast_to<T>(get_component(T::get_class_static()));
        }

        void _ready() override;

        void update_authority(const int32_t p_authority);
    };
}
