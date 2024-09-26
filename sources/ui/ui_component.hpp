#pragma once

#include <godot_cpp/classes/control.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class UiComponent : public godot::Control
    {
        GD_CLASS(UiComponent, Control);

    protected:
        
        static void _bind_methods();

    public:

        virtual void initialize();
        virtual void deinitialize();
    };
}
