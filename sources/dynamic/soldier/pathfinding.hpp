#pragma once

#include <shared/components/component.hpp>
#include <shared/util.hpp>

#include <shared/units/sleep_block.hpp>

namespace godot
{
    class NavigationAgent3D;
}

namespace recusant
{
    class SoldierPathfinding : public Component
    {
        GD_CLASS(SoldierPathfinding, Component);

    private:
        godot::Ref<SleepBlock> _process_block;
        godot::NavigationAgent3D* _agent = nullptr;
        godot::Node3D* _raycasts_parent = nullptr;
        float _movement_speed = 2.0f;
        float _lerp_factor = 0.25f;
        float _movement_delta = 0.0f;
        godot::Array _raycasts;
        SoldierSnapshot* _snapshot = nullptr;
        double _physics_delta = 0.0f;
        float _gravity = 0.0f;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY_NONCONST(godot::NavigationAgent3D*, agent);
        IMPL_PROPERTY_NONCONST(godot::Node3D*, raycasts_parent);
        IMPL_PROPERTY(float, movement_speed);
        IMPL_PROPERTY(float, lerp_factor);

        void set_movement_target(godot::Vector3 p_movement_target);
        void on_velocity_computed(godot::Vector3 p_safe_velocity);

        void _ready() override;
        void _process(double p_delta) override;
        void _physics_process(double p_delta) override;
    };
}
