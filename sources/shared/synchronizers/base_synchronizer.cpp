#include <shared/entity.hpp>

#include <shared/synchronizers/base_synchronizer.hpp>

using namespace godot;

namespace recusant
{
    void BaseSynchronizer::_bind_methods()
    {
        BIND_PROPERTY_HINTED(Variant::FLOAT, on_change_tps, PROPERTY_HINT_RANGE,
            godot::String::num_real(MIN_TPS) + "," + godot::String::num_real(MAX_TPS) + ",0.01");

        BIND_PROPERTY_HINTED(Variant::FLOAT, always_tps, PROPERTY_HINT_RANGE,
            godot::String::num_real(MIN_TPS) + "," + godot::String::num_real(MAX_TPS) + ",0.01");

        ADD_SIGNAL(MethodInfo("on_change_tps_changed", PropertyInfo(Variant::FLOAT, "p_on_change_tps")));
        ADD_SIGNAL(MethodInfo("always_tps_changed", PropertyInfo(Variant::FLOAT, "p_always_tps")));
    }

    Entity* BaseSynchronizer::get_entity()
    {
        if (_entity == nullptr)
        {
            _entity = Util::get_entity(this);
        }
        return _entity;
    }

    void BaseSynchronizer::_ready()
    {
        RUNTIME_ONLY();
        set_delta_interval(1.0 / _on_change_tps);
        emit_signal(on_change_tps_changed, _on_change_tps);

        set_replication_interval(1.0 / _always_tps);
        emit_signal(always_tps_changed, _always_tps);
    }
}
