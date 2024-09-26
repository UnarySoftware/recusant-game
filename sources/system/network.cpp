#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <system/launch_args.hpp>
#include <system/steam.hpp>
#include <system/logger.hpp>

#include <system/network.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(Network);

    void Network::_bind_methods()
    {
        BIND_SINGLETON();
    }

    void Network::start_network(bool p_server, String p_host_name)
    {
        _enet_peer.instantiate();

        if (p_server)
        {
            _enet_peer->create_server(40404);
            Log::print() << "Server listening on localhost port " << 40404 << Log::end();
        }
        else
        {
            _enet_peer->create_client(p_host_name, 40404);
        }

        get_multiplayer()->set_multiplayer_peer(_enet_peer);
    }

    void Network::start_network_steam(bool p_server, uint64_t p_host_steamid)
    {
        _steam_peer.instantiate();
        _steam_peer->set(Steam::get_singleton());

        if (p_server)
        {
            _steam_peer->create_server();
            Log::print() << "Steam P2P server listening on localhost port " << 44 << Log::end();
        }
        else
        {
            _steam_peer->create_client(p_host_steamid);
        }

        get_multiplayer()->set_multiplayer_peer(_steam_peer);
    }

    void Network::initialize()
    {
        ASSIGN_SINGLETON();

        _args = LaunchArgs::get_singleton();

        if (_args->is_steam())
        {
            if (_args->is_host())
            {
                start_network_steam(true, 0);
            }
            else
            {
                uint64_t target_steamid = static_cast<uint64_t>(_args->get_host().to_int());
                if (target_steamid == Steam::get_singleton()->GetSteamID64())
                {
                    Log::error() << "Tried to connecting to ourself!. Abort!" << Log::end();
                    get_tree()->quit(1);
                    return;
                }
                else if (target_steamid == 0)
                {
                    Log::error() << "Invalid target STEAM_ID! Are you sure you changed launch args in VSCode's launch.json?" << Log::end();
                    get_tree()->quit(1);
                    return;
                }
                start_network_steam(false, target_steamid);
            }
        }
        else
        {
            if (_args->is_host())
            {
                start_network(true, _args->get_host());
            }
            else
            {
                start_network(false, _args->get_host());
            }
        }
    }

    void Network::deinitialize()
    {
        UNASSIGN_SINGLETON();
    }

    godot::Ref<godot::MultiplayerPeer> Network::get_any_peer()
    {
        return get_multiplayer()->get_multiplayer_peer();
    }

    const godot::Ref<godot::ENetMultiplayerPeer> Network::get_enet_peer()
    {
        if (_args->is_steam())
        {
            return godot::Ref<ENetMultiplayerPeer>();
        }
        return _enet_peer;
    }

    const godot::Ref<SteamMultiplayerPeer> Network::get_steam_peer()
    {
        if (!_args->is_steam())
        {
            return godot::Ref<SteamMultiplayerPeer>();
        }
        return _steam_peer;
    }
}
