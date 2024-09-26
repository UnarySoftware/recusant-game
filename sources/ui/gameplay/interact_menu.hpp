#pragma once

#include <ui/ui_component.hpp>

#include <godot_cpp/templates/hash_map.hpp>
#include <shared/util.hpp>

namespace godot
{
    class Label;
    class VBoxContainer;
}

namespace recusant
{
    class InteractMenu : public UiComponent
    {
        GD_CLASS(InteractMenu, UiComponent);

    private:

        DEFINE_SINGLETON();

        godot::VBoxContainer* _root = nullptr;
        godot::Label* _option = nullptr;
        int _selected_option = 0;

        void _select(int p_index);
        void _select_higher();
        void _select_lower();

    protected:

        static void _bind_methods();

    public:

        IMPL_SINGLETON();

        godot::StringName on_interact;

        IMPL_PROPERTY_NONCONST(godot::VBoxContainer*, root);
        IMPL_PROPERTY_NONCONST(godot::Label*, option);

        InteractMenu()
        {
            on_interact = "on_interact";
        }

        void initialize() override;
        void deinitialize() override;
        void _process(double p_delta) override;

        void show(const godot::HashMap<godot::String, godot::Callable>& p_input);
        void hide();
    };
}
