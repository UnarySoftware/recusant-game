#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>

#include <shared/entity.hpp>
#include <shared/synchronizers/authority_synchronizer.hpp>

#include <system/player/spawner.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(PlayerSpawner);

    void PlayerSpawner::_bind_methods()
    {
        BIND_SINGLETON();

        BIND_PROPERTY_PATH(Node, players);
        BIND_PROPERTY_PATH_RESOURCE(PackedScene, player_scene);

        BIND_METHOD(create_player);
        BIND_METHOD(destroy_player);
        BIND_METHOD(spawn_host);
    }

    void PlayerSpawner::spawn_host()
    {
        create_player(SERVER_PEER);
    }

    void PlayerSpawner::initialize()
    {
        ASSIGN_SINGLETON();

        if (get_multiplayer()->is_server())
        {
            get_multiplayer()->connect("peer_connected", Callable(this, NAMEOF(create_player)));
            get_multiplayer()->connect("peer_disconnected", Callable(this, NAMEOF(destroy_player)));

            call_deferred(NAMEOF(spawn_host));
        }
    }

    void PlayerSpawner::deinitialize()
    {
        UNASSIGN_SINGLETON();
    }

    void PlayerSpawner::create_player(int p_id)
    {
        Entity* NewPlayer = cast_to<Entity>(_player_scene->instantiate());
        NewPlayer->set_name(String::num_int64(p_id));
        _players->add_child(NewPlayer);
        NewPlayer->get_authority_synchronizer()->set_authority(p_id);
    }

    void PlayerSpawner::destroy_player(int p_id)
    {
        _players->get_node<Node>(String::num_int64(p_id))->queue_free();
    }
}
