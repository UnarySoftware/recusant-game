#include <shared/synchronizers/entity_synchronizer.hpp>

using namespace godot;

namespace recusant
{
    void EntitySynchronizer::_bind_methods()
    {
        
    }

    void EntitySynchronizer::_ready()
    {
        RUNTIME_ONLY();

        Super::_ready();
        
        // TODO Remove this when moving to visibility based zoning
        //set_visibility_public(true);
        //set_visibility_update_mode(VisibilityUpdateMode::VISIBILITY_PROCESS_NONE);

        // TODO Add NodePath leading to what would be considered as a position to the object for visibility calculations:
        // Door -> Entity as root (since does not have custom movement code)
        // Player/NPC -> CharacterBody3D (since it has custom movement code)

    }

    void EntitySynchronizer::set_sleep(bool p_value)
    {
        if (p_value)
        {
            set_delta_interval(1.0 / SLEEP_TPS);
            emit_signal(on_change_tps_changed, SLEEP_TPS);

            set_replication_interval(1.0 / SLEEP_TPS);
            emit_signal(always_tps_changed, SLEEP_TPS);
        }
        else
        {
            set_delta_interval(1.0 / _on_change_tps);
            emit_signal(on_change_tps_changed, _on_change_tps);

            set_replication_interval(1.0 / _always_tps);
            emit_signal(always_tps_changed, _always_tps);
        }
    }
}
