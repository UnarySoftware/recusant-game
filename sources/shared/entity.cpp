#include <system/logger.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <shared/synchronizers/authority_synchronizer.hpp>
#include <shared/synchronizers/entity_synchronizer.hpp>

#include <shared/entity.hpp>

using namespace godot;

namespace recusant
{
    AuthoritySynchronizer* Entity::get_authority_synchronizer()
    {
        if (!_got_components)
        {
            _get_components();
        }
        return _authority_synchronizer;
    }

    EntitySynchronizer* Entity::get_entity_synchronizer()
    {
        if (!_got_components)
        {
            _get_components();
        }
        return _entity_synchronizer;
    }

    void Entity::_get_components()
    {
        ClassDBSingleton* class_db = ClassDBSingleton::get_singleton();
        int child_count = get_child_count();
        for (int i = 0; i < child_count; ++i)
        {
            Node* node = get_child(i);
            Component* component = cast_to<Component>(node);
            if (component == nullptr)
            {
                if (AuthoritySynchronizer* authority_synchronizer = cast_to<AuthoritySynchronizer>(node))
                {
                    _authority_synchronizer = authority_synchronizer;
                }
                else if (EntitySynchronizer* entity_synchronizer = cast_to<EntitySynchronizer>(node))
                {
                    _entity_synchronizer = entity_synchronizer;
                }
                continue;
            }

            auto itterator = component->get_class();
            if (itterator.is_empty())
            {
                continue;
            }

            if (_components.has(itterator))
            {
                Log::error() << "Tried re-registering " << itterator << " class as a component in a node named " << get_name() << Log::end();
                continue;
            }
            _components[itterator] = component;

            while (!itterator.is_empty())
            {
                itterator = class_db->get_parent_class(itterator);
                if (itterator == StringName("Component"))
                {
                    break;
                }
                if (_components.has(itterator))
                {
                    Log::error() << "Tried re-registering " << itterator << " class as a component in a node named " << get_name() << Log::end();
                    break;
                }
                _components[itterator] = component;
            }
        }

        _got_components = true;
    }

    void Entity::_bind_methods()
    {
        ADD_SIGNAL(MethodInfo("on_authority_change", PropertyInfo(Variant::INT, "p_authority")));
    }

    void Entity::_ready()
    {
        // This is necessary, since emmiting signals / traversing scenetree
        // requires this node to be ready to do so first
        _is_ready = true;
        if (_authority != SERVER_PEER)
        {
            update_authority(_authority);
        }
    }

    void Entity::update_authority(const int32_t p_authority)
    {
        _authority = p_authority;

        if (!_is_ready)
        {
            return;
        }

        int32_t children = get_child_count();

        for (int32_t i = 0; i < children; ++i)
        {
            Node* child = get_child(i);
            if (Component* component = cast_to<Component>(child))
            {
                component->set_multiplayer_authority(p_authority, true);
                component->on_authority_changed();
            }
            else if (EntitySynchronizer* entity_synchronizer = cast_to<EntitySynchronizer>(child))
            {
                entity_synchronizer->set_multiplayer_authority(p_authority, true);
            }
            else if (AuthoritySynchronizer* authority_synchronizer = cast_to<AuthoritySynchronizer>(child))
            {
                // AuthoritySynchronizer based component is supposed to be always owned by server
                authority_synchronizer->set_multiplayer_authority(SERVER_PEER, true);
            }
        }

        emit_signal(on_authority_change, p_authority);
    }
}
