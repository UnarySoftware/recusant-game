#pragma once

#include <godot_cpp/classes/e_net_multiplayer_peer.hpp>
#include <system/units/steam_multiplayer_peer.hpp>

#include <system/system.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class LaunchArgs;

    class Network : public System
    {
        GD_CLASS(Network, System);

    private:

        DEFINE_SINGLETON();
        
        LaunchArgs* _args = nullptr;
        godot::Ref<godot::ENetMultiplayerPeer> _enet_peer;
        godot::Ref<SteamMultiplayerPeer> _steam_peer;

    protected:

        static void _bind_methods();

    public:

        IMPL_SINGLETON();

        void start_network(bool p_server, godot::String p_host_name);
        void start_network_steam(bool p_server, uint64_t p_host_steamid);

        void initialize() override;
        void deinitialize() override;

        godot::Ref<godot::MultiplayerPeer> get_any_peer();
        const godot::Ref<godot::ENetMultiplayerPeer> get_enet_peer();
        const godot::Ref<SteamMultiplayerPeer> get_steam_peer();
    };
}
