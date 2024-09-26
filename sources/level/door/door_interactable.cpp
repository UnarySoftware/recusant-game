#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>
#include <shared/entity.hpp>
#include <shared/components/snapshot.hpp>

#include <level/door/door_interactable.hpp>

using namespace godot;

namespace recusant
{
    void DoorInteractable::_bind_methods()
    {
        BIND_ENUM_CONSTANT(None);
        BIND_ENUM_CONSTANT(Forward);
        BIND_ENUM_CONSTANT(Backward);

        BIND_METHOD(send_direction);

        BIND_PROPERTY_HINTED(Variant::INT, direction, PROPERTY_HINT_ENUM, "None,Forward,Backward");

        BIND_PROPERTY_PATH(Node3D, hinge);

        BIND_METHOD(try_open);

        BIND_PROPERTY(Variant::FLOAT, speed);
        BIND_PROPERTY(Variant::FLOAT, speed_up);
    }

    DoorInteractable::OpenDirection DoorInteractable::get_direction()
    {
        return _direction;
    }

    void DoorInteractable::set_direction(DoorInteractable::OpenDirection p_direction)
    {
        _direction = p_direction;

        RUNTIME_ONLY();

        _weight = _speed;

        if (_direction == OpenDirection::Forward)
        {
            _rotation = Vector3(0.0f, 90.0f, 0.0f);
        }
        else if (_direction == OpenDirection::Backward)
        {
            _rotation = Vector3(0.0f, -90.0f, 0.0f);
        }
        else
        {
            _rotation = Vector3(0.0f, 0.0f, 0.0f);
        }
    }

    void DoorInteractable::send_direction(DoorInteractable::OpenDirection p_direction)
    {
        set_direction(p_direction);
    }

    void DoorInteractable::_ready()
    {
        RUNTIME_ONLY();

        add_option("open", Callable(this, "try_open"));

        BIND_RPC(send_direction, MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TransferMode::TRANSFER_MODE_RELIABLE, true);

        if (is_multiplayer_authority())
        {
            RPC_TARGET(SERVER_PEER, NAMEOF(send_direction), _direction);
        }
    }

    void DoorInteractable::_process(double p_delta)
    {
        RUNTIME_ONLY();

        if (!_hinge->get_rotation_degrees().is_equal_approx(_rotation))
        {
            _weight += _speed_up;
            _hinge->set_rotation_degrees(_hinge->get_rotation_degrees().lerp(_rotation, p_delta * _weight));
        }
    }

    void DoorInteractable::try_open(Entity* p_opener)
    {
        Vector3 position;
        Snapshot* snapshot = p_opener->get_component<Snapshot>();
        if (snapshot == nullptr)
        {
            position = p_opener->get_position();
        }
        else
        {
            position = snapshot->get_position();
        }

        if (_direction == OpenDirection::None)
        {
            if (get_entity()->to_local(position).x > 0.0f)
            {
                RPC_TARGET(SERVER_PEER, NAMEOF(send_direction), OpenDirection::Backward);
            }
            else
            {
                RPC_TARGET(SERVER_PEER, NAMEOF(send_direction), OpenDirection::Forward);
            }
        }
        else
        {
            RPC_TARGET(SERVER_PEER, NAMEOF(send_direction), OpenDirection::None);
        }
    }
}
