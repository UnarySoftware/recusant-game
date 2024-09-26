#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/label3d.hpp>
#include <godot_cpp/classes/time.hpp>

#include <level/drill/drill_resources.hpp>

using namespace godot;

namespace recusant
{
    void DrillResources::_bind_methods()
    {
        BIND_PROPERTY(Variant::FLOAT, timestamp);
        BIND_PROPERTY(Variant::FLOAT, timestamp_collected);

        BIND_METHOD(take_resources);
        
        BIND_PROPERTY_PATH(Node3D, head);
        BIND_PROPERTY_PATH(Node3D, start_move);
        BIND_PROPERTY_PATH(Node3D, end_move);
        BIND_PROPERTY_PATH(Label3D, screen);
    }

    void DrillResources::update_collected()
    {
        double delta_time = Time::get_singleton()->get_unix_time_from_system() - _timestamp;
        _collected = _timestamp_collected + (delta_time * _step_generation);
        _screen->set_text(String::num_int64(UtilityFunctions::roundi(_collected)));
    }

    void DrillResources::take_resources(double p_amount)
    {
        _timestamp = Time::get_singleton()->get_unix_time_from_system();
        _timestamp_collected = Math::clamp(_collected - p_amount, 0.0, _buffer);
    }

    void DrillResources::_ready()
    {
        RUNTIME_ONLY();

        _timestamp = Time::get_singleton()->get_unix_time_from_system();

        BIND_RPC(take_resources, MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TransferMode::TRANSFER_MODE_RELIABLE, true);
        
        _process_sleep.instantiate();
        _process_sleep->set(4.0);
    }

    void DrillResources::_process(double p_delta)
    {
        RUNTIME_ONLY();

        if (_start_to_end)
        {
            _head->set_position(_head->get_position().lerp(_end_move->get_position(), 5.0f * p_delta));

            if (_head->get_position().is_equal_approx(_end_move->get_position()))
            {
                _start_to_end = false;
            }
        }
        else
        {
            _head->set_position(_head->get_position().lerp(_start_move->get_position(), 5.0f * p_delta));

            if (_head->get_position().is_equal_approx(_start_move->get_position()))
            {
                _start_to_end = true;
            }
        }

        if (!_process_sleep->update(p_delta))
        {
            return;
        }

        update_collected();
    }

}
