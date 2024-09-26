#include <ui/core/core_state.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(CoreState);

    void CoreState::_bind_methods()
    {
        BIND_SINGLETON();
    }

    void CoreState::initialize()
    {
        Super::initialize();

        ASSIGN_SINGLETON();
    }

    void CoreState::deinitialize()
    {
        Super::deinitialize();

        UNASSIGN_SINGLETON();
    }
}
