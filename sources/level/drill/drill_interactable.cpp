#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>
#include <shared/entity.hpp>
#include <shared/components/snapshot.hpp>

#include <level/drill/drill_resources.hpp>

#include <level/drill/drill_interactable.hpp>

using namespace godot;

namespace recusant
{
    void DrillInteractable::_bind_methods()
    {
        BIND_METHOD(get_resources);
    }

    void DrillInteractable::_ready()
    {
        RUNTIME_ONLY();

        _resources = get_component<DrillResources>();

        add_option("get resources", Callable(this, "get_resources"));
    }

    void DrillInteractable::get_resources(Entity* p_taker)
    {
        _resources->RPC_TARGET(SERVER_PEER, NAMEOF(take_resources), 2500.0);
    }
}
