#include <ui/ui_state.hpp>

#include <system/ui_root.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(UiRoot);

    void UiRoot::_bind_methods()
    {
        BIND_SINGLETON();
    }

    void UiRoot::initialize()
    {
        ASSIGN_SINGLETON();

        int32_t children = get_child_count();

        for (int32_t i = 0; i < children; ++i)
        {
            UiState* state = cast_to<UiState>(get_child(i));
            if (state)
            {
                String class_name = state->get_class();
                _state_map[class_name] = state;
                _state_vec.push_back(state);
                state->initialize();
            }
        }
    }

    void UiRoot::deinitialize()
    {
        for (int i = _state_vec.size() - 1; i >= 0; i--)
        {
            UiState* target = _state_vec[i];
            target->deinitialize();
        }

        UNASSIGN_SINGLETON();
    }
}
