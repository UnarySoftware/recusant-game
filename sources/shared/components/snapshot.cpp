#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>

#include <system/logger.hpp>
#include <shared/synchronizers/entity_synchronizer.hpp>

#include <shared/components/snapshot.hpp>

using namespace godot;

namespace recusant
{
    void Snapshot::_bind_methods()
    {
        BIND_PROPERTY(Variant::FLOAT, last_send);
        BIND_PROPERTY(Variant::VECTOR3, position);
        BIND_PROPERTY(Variant::VECTOR3, rotation);

        BIND_METHOD(on_synchronized);
    }

    void Snapshot::on_synchronized()
    {
        _generator->add_frame(get_last_send_without_reset(), get_position(), get_rotation());
    }

    void Snapshot::_ready()
    {
        RUNTIME_ONLY();

        _generator.instantiate();

        get_entity_synchronizer()->connect("synchronized", Callable(this, NAMEOF(on_synchronized)));
    }

    void Snapshot::_process(double p_delta)
    {
        RUNTIME_ONLY();
        if (is_multiplayer_authority())
        {
            _last_send += p_delta;
            set_position(provide_position());
            set_rotation(provide_rotation());
        }
        else
        {
            if (_generator->has_frames())
            {
                // Update playback and apply new position.
                _generator->update(p_delta);
                update_position(_generator->get_position());
                update_rotation(_generator->get_rotation().get_euler());
            }
        }
    }

    void Snapshot::set_last_send(const float p_last_send)
    {
        RUNTIME_ONLY();
        _last_send = p_last_send;
    }

    float Snapshot::get_last_send()
    {
        RUNTIME_ONLY(0.0f);
        float current_send = _last_send;
        _last_send = 0.0f;
        return current_send;
    }

    float Snapshot::get_last_send_without_reset()
    {
        return _last_send;
    }

    void Snapshot::update_position(const godot::Vector3 p_position)
    {

    }

    godot::Vector3 Snapshot::provide_position() const
    {
        return godot::zero::Vector3;
    }

    void Snapshot::update_rotation(const godot::Vector3 p_rotation)
    {

    }

    godot::Vector3 Snapshot::provide_rotation() const
    {
        return godot::zero::Vector3;
    }
}
