
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>

#include <dynamic/player/callout.hpp>

using namespace godot;

namespace recusant
{
    void PlayerCallout::_bind_methods()
    {
        BIND_PROPERTY_PATH(RayCast3D, raycast);
        BIND_PROPERTY_PATH(Node3D, callout);

        BIND_METHOD(set_new_callout);
    }

    void PlayerCallout::set_new_callout(Vector3 p_position)
    {
        _callout->set_visible(true);
        _callout->set_position(p_position);
    }

    void PlayerCallout::_ready()
    {
        RUNTIME_ONLY();

        BIND_RPC(set_new_callout, MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TransferMode::TRANSFER_MODE_RELIABLE, true);
    }

    void PlayerCallout::on_authority_changed()
    {
        set_process(is_multiplayer_authority());
    }

    void PlayerCallout::_process(double p_delta)
    {
        RUNTIME_ONLY();

        if (!Input::get_singleton()->is_action_just_pressed("ui_callout"))
        {
            return;
        }

        if (!_raycast->is_colliding())
        {
            return;
        }

        RPC_EVERYONE(NAMEOF(set_new_callout), _raycast->get_collision_point());
    }
}
