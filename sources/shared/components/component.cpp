#include <shared/components/component.hpp>
#include <shared/synchronizers/authority_synchronizer.hpp>
#include <shared/synchronizers/entity_synchronizer.hpp>

using namespace godot;

namespace recusant
{
    void Component::_bind_methods()
    {

    }

    Entity* Component::get_entity()
    {
        if (_entity == nullptr)
        {
            _entity = Util::get_entity(this);
        }
        return _entity;
    }

    Component* Component::get_component(const godot::StringName& p_component_name)
    {
        return get_entity()->get_component(p_component_name);
    }

    AuthoritySynchronizer* Component::get_authority_synchronizer()
    {
        return get_entity()->get_authority_synchronizer();
    }

    EntitySynchronizer* Component::get_entity_synchronizer()
    {
        return get_entity()->get_entity_synchronizer();
    }
}
