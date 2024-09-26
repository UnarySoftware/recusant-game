#include <godot_cpp/classes/scene_tree.hpp>
#include <system/units/steam_callback.hpp>
#include <system/launch_args.hpp>
#include <system/logger.hpp>

#include <system/steam.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(Steam);

    void Steam::_bind_methods()
    {
        BIND_SINGLETON();

        ADD_SIGNAL(MethodInfo("SteamNetConnectionStatusChangedSignal", PropertyInfo(Variant::OBJECT, "callback")));
    }

    // Callbacks

    void Steam::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t *p_callback)
    {
        Ref<SteamCallback> callback;
        callback.instantiate();
        callback->set(SteamCallback::CallbackType::SteamNetConnectionStatusChangedCallback, reinterpret_cast<uint64_t *>(p_callback));
        emit_signal(SteamNetConnectionStatusChangedSignal, callback);
    }

    // SteamAPI

    bool Steam::Init()
    {
        return SteamAPI_Init();
    }

    void Steam::Shutdown()
    {
        SteamAPI_Shutdown();
    }

    bool Steam::RestartAppIfNecessary(uint32 unOwnAppID)
    {
        return SteamAPI_RestartAppIfNecessary(unOwnAppID);
    }

    void Steam::ReleaseCurrentThreadMemory()
    {
        SteamAPI_ReleaseCurrentThreadMemory();
    }
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

    HSteamListenSocket Steam::CreateListenSocketP2P(int nVirtualPort, int nOptions, const SteamNetworkingConfigValue_t *pOptions)
    {
        if (!_initialized)
        {
            return 0;
        }
        return SteamNetworkingSockets()->CreateListenSocketP2P(nVirtualPort, nOptions, pOptions);
    }

    HSteamNetConnection Steam::ConnectP2P(const SteamNetworkingIdentity &identityRemote, int nVirtualPort, int nOptions, const SteamNetworkingConfigValue_t *pOptions)
    {
        if (!_initialized)
        {
            return 0;
        }
        return SteamNetworkingSockets()->ConnectP2P(identityRemote, nVirtualPort, nOptions, pOptions);
    }

    EResult Steam::AcceptConnection(HSteamNetConnection hConn)
    {
        if (!_initialized)
        {
            return k_EResultDisabled;
        }
        return SteamNetworkingSockets()->AcceptConnection(hConn);
    }

    bool Steam::CloseConnection(HSteamNetConnection hPeer, int nReason, const char *pszDebug, bool bEnableLinger)
    {
        if (!_initialized)
        {
            return false;
        }
        return SteamNetworkingSockets()->CloseConnection(hPeer, nReason, pszDebug, bEnableLinger);
    }

    bool Steam::CloseListenSocket(HSteamListenSocket hSocket)
    {
        if (!_initialized)
        {
            return false;
        }
        return SteamNetworkingSockets()->CloseListenSocket(hSocket);
    }

    EResult Steam::SendMessageToConnection(HSteamNetConnection hConn, const void *pData, uint32 cbData, int nSendFlags, int64 *pOutMessageNumber)
    {
        if (!_initialized)
        {
            return k_EResultDisabled;
        }
        return SteamNetworkingSockets()->SendMessageToConnection(hConn, pData, cbData, nSendFlags, pOutMessageNumber);
    }

    int Steam::ReceiveMessagesOnConnection(HSteamNetConnection hConn, SteamNetworkingMessage_t **ppOutMessages, int nMaxMessages)
    {
        if (!_initialized)
        {
            return 0;
        }
        return SteamNetworkingSockets()->ReceiveMessagesOnConnection(hConn, ppOutMessages, nMaxMessages);
    }

    HSteamNetPollGroup Steam::CreatePollGroup()
    {
        if (!_initialized)
        {
            return 0;
        }
        return SteamNetworkingSockets()->CreatePollGroup();
    }

    bool Steam::DestroyPollGroup(HSteamNetPollGroup hPollGroup)
    {
        if (!_initialized)
        {
            return false;
        }
        return SteamNetworkingSockets()->DestroyPollGroup(hPollGroup);
    }

    bool Steam::SetConnectionPollGroup(HSteamNetConnection hConn, HSteamNetPollGroup hPollGroup)
    {
        if (!_initialized)
        {
            return false;
        }
        return SteamNetworkingSockets()->SetConnectionPollGroup(hConn, hPollGroup);
    }

    int Steam::ReceiveMessagesOnPollGroup(HSteamNetPollGroup hPollGroup, SteamNetworkingMessage_t **ppOutMessages, int nMaxMessages)
    {
        if (!_initialized)
        {
            return 0;
        }
        return SteamNetworkingSockets()->ReceiveMessagesOnPollGroup(hPollGroup, ppOutMessages, nMaxMessages);
    }

    // ISteamNetworkingUtils Interface

    void Steam::InitRelayNetworkAccess()
    {
        if (!_initialized)
        {
            return;
        }
        SteamNetworkingUtils()->InitRelayNetworkAccess();
    }

    // ISteamParties Interface
    // ISteamRemotePlay Interface
    // ISteamRemoteStorage Interface
    // ISteamScreenshots Interface
    // ISteamUGC Interface

    // ISteamUser Interface

    CSteamID Steam::GetSteamID()
    {
        if (!_initialized)
        {
            return CSteamID();
        }
        return SteamUser()->GetSteamID();
    }

    uint64_t Steam::GetSteamID64()
    {
        if (!_initialized)
        {
            return 0;
        }
        return SteamUser()->GetSteamID().ConvertToUint64();
    }

    uint64_t Steam::GetSteamID64(CSteamID SteamID)
    {
        if (!_initialized)
        {
            return 0;
        }
        return SteamID.ConvertToUint64();
    }

    // ISteamUserStats Interface
    // ISteamUtils Interface
    // ISteamVideo Interface

    // Godot

    void Steam::initialize()
    {
        ASSIGN_SINGLETON();

        if (!LaunchArgs::get_singleton()->is_steam())
        {
            return;
        }

        if (RestartAppIfNecessary(app_id))
        {
            Log::print() << "Steam asked us to restart an app.";
            get_tree()->quit(0);
            return;
        }

        _initialized = Init();

        if (!_initialized)
        {
            Log::error() << "Failed to initialize Steam.\nTry restarting the app and/or Steam.";
            return;
        }

        InitRelayNetworkAccess();
    }

    void Steam::_process(double p_delta)
    {
        if (_initialized)
        {
            SteamAPI_RunCallbacks();
        }
    }

    void Steam::deinitialize()
    {
        if (_initialized)
        {
            SteamAPI_Shutdown();
        }

        UNASSIGN_SINGLETON();
    }

}
