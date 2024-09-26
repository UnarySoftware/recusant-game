#pragma once

// TODO Unfortunatelly, you cannot get rid of this HUGE include without using something like PImpl, 
// and I just cant be bothered to fix it for now :(
#include <steam/steam_api.h>

#include <system/system.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class Steam : public System
    {
        GD_CLASS(Steam, System);
            
    private:

        DEFINE_SINGLETON();
        
        bool _initialized = false;

    protected:

        static void _bind_methods();

    public:

        const int app_id = 1436420;

        godot::StringName SteamNetConnectionStatusChangedSignal;

        IMPL_SINGLETON();

        Steam()
        {
            SteamNetConnectionStatusChangedSignal = "SteamNetConnectionStatusChangedSignal";
        }

        // Callbacks
        STEAM_CALLBACK(Steam, SteamNetConnectionStatusChangedCallback, SteamNetConnectionStatusChangedCallback_t);

        // SteamAPI
        bool Init();
        void Shutdown();
        bool RestartAppIfNecessary(uint32 unOwnAppID);
        void ReleaseCurrentThreadMemory();
        // ISteamApps Interface
        // ISteamAppTicket Interface
        // ISteamClient Interface
        // ISteamFriends Interface
        // ISteamGameCoordinator Interface
        // ISteamGameServer Interface
        // ISteamGameServerStats Interface
        // ISteamHTMLSurface Interface
        // ISteamHTTP Interface
        // ISteamInput Interface
        // ISteamInventory Interface
        // ISteamMatchmaking Interface
        // ISteamMatchmakingServers Interface
        // ISteamMusic Interface
        // ISteamMusicRemote Interface
        // ISteamNetworking Interface
        // ISteamNetworkingMessages Interface
        // ISteamNetworkingSockets Interface
        HSteamListenSocket CreateListenSocketP2P(int nVirtualPort, int nOptions, const SteamNetworkingConfigValue_t *pOptions);
        HSteamNetConnection ConnectP2P(const SteamNetworkingIdentity &identityRemote, int nVirtualPort, int nOptions, const SteamNetworkingConfigValue_t *pOptions);
        EResult AcceptConnection(HSteamNetConnection hConn);
        bool CloseConnection(HSteamNetConnection hPeer, int nReason, const char *pszDebug, bool bEnableLinger);
        bool CloseListenSocket(HSteamListenSocket hSocket);
        EResult SendMessageToConnection(HSteamNetConnection hConn, const void *pData, uint32 cbData, int nSendFlags, int64 *pOutMessageNumber);
        int ReceiveMessagesOnConnection(HSteamNetConnection hConn, SteamNetworkingMessage_t **ppOutMessages, int nMaxMessages);
        HSteamNetPollGroup CreatePollGroup();
        bool DestroyPollGroup(HSteamNetPollGroup hPollGroup);
        bool SetConnectionPollGroup(HSteamNetConnection hConn, HSteamNetPollGroup hPollGroup);
        int ReceiveMessagesOnPollGroup(HSteamNetPollGroup hPollGroup, SteamNetworkingMessage_t **ppOutMessages, int nMaxMessages);
        // ISteamNetworkingUtils Interface
        void InitRelayNetworkAccess();
        // ISteamParties Interface
        // ISteamRemotePlay Interface
        // ISteamRemoteStorage Interface
        // ISteamScreenshots Interface
        // ISteamUGC Interface
        // ISteamUser Interface
        CSteamID GetSteamID();
        uint64_t GetSteamID64();
        uint64_t GetSteamID64(CSteamID SteamID);
        // ISteamUserStats Interface
        // ISteamUtils Interface
        // ISteamVideo Interface
        // Godot
        void initialize() override;
        void deinitialize() override;
        void _process(double p_delta) override;
    };
}
