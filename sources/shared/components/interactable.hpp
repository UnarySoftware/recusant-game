#pragma once

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class Interactable : public Component
    {
        GD_CLASS(Interactable, Component);

    private:

        godot::HashMap<godot::String, godot::Callable> _options;

    protected:
        
        static void _bind_methods() {}

        void add_option(const godot::String &p_option_name, const godot::Callable &p_callable);
        void clear_option();

    public:
        
        const godot::HashMap<godot::String, godot::Callable> get_options();

    };
}
