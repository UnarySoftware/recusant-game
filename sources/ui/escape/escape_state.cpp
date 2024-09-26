#include <ui/escape/escape_state.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(EscapeState);

    void EscapeState::_bind_methods()
    {
        BIND_SINGLETON();
    }

    void EscapeState::initialize()
    {
        Super::initialize();

        ASSIGN_SINGLETON();
    }

    void EscapeState::deinitialize()
    {
        Super::deinitialize();

        UNASSIGN_SINGLETON();
    }
}
