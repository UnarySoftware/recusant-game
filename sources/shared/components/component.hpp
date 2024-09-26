#pragma once

#include <shared/entity.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class Entity;
    class AuthoritySynchronizer;
    class EntitySynchronizer;

    class Component : public godot::Node
    {
        GD_CLASS(Component, Node);

    private:

        friend class Entity;
        Entity* _entity = nullptr;

    protected:
        
        static void _bind_methods();

        Entity* get_entity();

        Component* get_component(const godot::StringName& p_component_name);
        
        template <class T>
        T* get_component()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit Component to be used with get_component()");
            return cast_to<T>(get_component(T::get_class_static()));
        }

        AuthoritySynchronizer* get_authority_synchronizer();
        EntitySynchronizer* get_entity_synchronizer();

        virtual void on_authority_changed() {};
    };
}
