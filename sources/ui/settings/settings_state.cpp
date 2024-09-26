#include <ui/settings/settings_state.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(SettingsState);

    void SettingsState::_bind_methods()
    {
        BIND_SINGLETON();
    }

    void SettingsState::initialize()
    {
        Super::initialize();

        ASSIGN_SINGLETON();
    }

    void SettingsState::deinitialize()
    {
        Super::deinitialize();

        UNASSIGN_SINGLETON();
    }
}
