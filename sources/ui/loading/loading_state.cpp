#include <ui/loading/loading_state.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(LoadingState);

    void LoadingState::_bind_methods()
    {
        BIND_SINGLETON();
    }

    void LoadingState::initialize()
    {
        Super::initialize();

        ASSIGN_SINGLETON();
    }

    void LoadingState::deinitialize()
    {
        Super::deinitialize();

        UNASSIGN_SINGLETON();
    }
}
