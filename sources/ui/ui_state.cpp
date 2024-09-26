#include <ui/ui_component.hpp>

#include <ui/ui_state.hpp>

using namespace godot;

namespace recusant
{
    void UiState::_bind_methods()
    {
        
    }

    void UiState::initialize()
    {
        int32_t children = get_child_count();

        for (int32_t i = 0; i < children; ++i)
        {
            UiComponent* comp = cast_to<UiComponent>(get_child(i));
            if (comp)
            {
                String class_name = comp->get_class();
                _comp_map[class_name] = comp;
                _comp_vec.push_back(comp);
                comp->initialize();
            }
        }
    }

    void UiState::deinitialize()
    {
        for (int i = _comp_vec.size() - 1; i >= 0; i--)
        {
            UiComponent* target = _comp_vec[i];
            target->deinitialize();
        }
    }
}
