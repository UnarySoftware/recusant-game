#include <shared/entity.hpp>
#include <godot_cpp/classes/scene_replication_config.hpp>

#include <shared/synchronizers/authority_synchronizer.hpp>

using namespace godot;

namespace recusant
{
    void AuthoritySynchronizer::_bind_methods()
    {
        BIND_PROPERTY(Variant::INT, authority);
    }

    void AuthoritySynchronizer::set_authority(const int32_t p_authority)
    {
        RUNTIME_ONLY();
        _authority = p_authority;
        get_entity()->update_authority(p_authority);
    }

    int32_t AuthoritySynchronizer::get_authority() const
    {
        RUNTIME_ONLY(0);
        return _authority;
    }

    void AuthoritySynchronizer::_ready()
    {
        RUNTIME_ONLY();

        // We always fire authority updating process, since it might never be updated
        // automatically. Example: Host owned authority synchronizer always equals SERVER_PEER, which means
        // that Synchronizer never synchs value since there was no change, resulting in never triggering the update.
        set_authority(SERVER_PEER);

        // Do not call Super::_ready() here, since AuthoritySynchronizer
        // will want to synchronize AS SOON AS POSSIBLE, ignoring all range
        // checks ( MIN_TPS / MAX_TPS )
        set_delta_interval(0.0);
        emit_signal(on_change_tps_changed, _on_change_tps);
    }

    void AuthoritySynchronizer::set_sleep(bool p_value)
    {
        if (p_value)
        {
            set_delta_interval(1.0 / SLEEP_TPS);
            emit_signal(on_change_tps_changed, SLEEP_TPS);
        }
        else
        {
            set_delta_interval(0.0);
            emit_signal(on_change_tps_changed, _on_change_tps);
        }
    }
}
