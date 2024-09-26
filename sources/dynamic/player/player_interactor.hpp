#pragma once

#include <shared/components/component.hpp>
#include <shared/util.hpp>

#include <shared/units/sleep_block.hpp>

namespace recusant
{
    class InteractMenu;

    class PlayerInteractor : public Component
    {
        GD_CLASS(PlayerInteractor, Component);

    private:

        godot::Callable _interact_callable;
        godot::Ref<SleepBlock> _process_sleep;
        godot::RayCast3D* _raycast = nullptr;
        Interactable* _collision_target = nullptr;
        PlayerSnapshot* _snapshot = nullptr;
        InteractMenu* _interact_menu = nullptr;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY_NONCONST(godot::RayCast3D*, raycast);

        void on_interact(godot::String p_option);
        void _ready() override;
        void on_authority_changed() override;
        void _process(double p_delta) override;
    };
}
