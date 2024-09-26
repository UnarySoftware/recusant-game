#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>
#include <godot_cpp/classes/area3d.hpp>

#include <level/trigger_multiple.hpp>

using namespace godot;

namespace recusant
{
    void TriggerMultiple::_bind_methods()
    {
        BIND_PROPERTY_PATH(Area3D, trigger);
        BIND_PROPERTY(Variant::FLOAT, sleep_seconds);
        BIND_PROPERTY(Variant::FLOAT, active_seconds);
        BIND_METHOD(on_process_active);
        BIND_METHOD(on_process_sleep);
    }

    void TriggerMultiple::_ready()
    {
        RUNTIME_ONLY();

        if (!is_multiplayer_authority())
        {
            queue_free();
            return;
        }

        set_process(is_multiplayer_authority());

        _process_sleep.instantiate();
        _process_sleep->set(_sleep_seconds, _active_seconds);
        _process_sleep->connect(_process_sleep->on_active, Callable(this, NAMEOF(on_process_active)));
        _process_sleep->connect(_process_sleep->on_sleep, Callable(this, NAMEOF(on_process_sleep)));
    }

    void TriggerMultiple::on_process_active()
    {
        _trigger->set_monitoring(true);
    }

    void TriggerMultiple::on_process_sleep()
    {
        _trigger->set_monitoring(false);
    }

    void TriggerMultiple::_process(double p_delta)
    {
        RUNTIME_ONLY();

        if (!_trigger->is_monitoring() || !_trigger->has_overlapping_bodies())
        {
            _process_sleep->update(p_delta);
        }
    }
}
