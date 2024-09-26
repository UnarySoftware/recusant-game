#include <ui/gameplay/gameplay_state.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(GameplayState);

    void GameplayState::_bind_methods()
    {
        BIND_SINGLETON();
    }

    void GameplayState::initialize()
    {
        Super::initialize();

        ASSIGN_SINGLETON();
    }

    void GameplayState::deinitialize()
    {
        Super::deinitialize();

        UNASSIGN_SINGLETON();
    }
}
