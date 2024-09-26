#pragma once

#include <godot_cpp/classes/control.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class UiComponent;

    class UiState : public godot::Control
    {
        GD_CLASS(UiState, Control);

    private:

        std::vector<UiComponent*> _comp_vec;
        std::unordered_map<godot::String, UiComponent*, godot::hash::String> _comp_map;

    protected:
        
        static void _bind_methods();

    public:

        virtual void initialize();
        virtual void deinitialize();

    };
}
