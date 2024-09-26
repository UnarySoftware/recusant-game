#pragma once

#include <shared/components/component.hpp>
#include <shared/units/sleep_block.hpp>
#include <shared/util.hpp>

namespace godot
{
    class Label3D;
}

namespace recusant
{
    class DrillResources : public Component
    {
        GD_CLASS(DrillResources, Component);

    private:

        godot::Ref<SleepBlock> _process_sleep;

        double _timestamp = 0.0;
        double _timestamp_collected = 0.0;

        double _buffer = 3600.0;
        double _collected = 0.0;
        double _step_generation = 85.0;

        godot::Node3D* _head = nullptr;
        godot::Node3D* _start_move = nullptr;
        godot::Node3D* _end_move = nullptr;
        godot::Label3D* _screen = nullptr;

        bool _start_to_end = true;

    protected:

        static void _bind_methods();

    public:

        IMPL_PROPERTY(double, timestamp);
        IMPL_PROPERTY(float, timestamp_collected);

        IMPL_PROPERTY_NONCONST(godot::Node3D*, head);
        IMPL_PROPERTY_NONCONST(godot::Node3D*, start_move);
        IMPL_PROPERTY_NONCONST(godot::Node3D*, end_move);
        IMPL_PROPERTY_NONCONST(godot::Label3D*, screen);

        void update_collected();

        void take_resources(double p_amount);

        void _ready() override;
        void _process(double p_delta) override;
    };
}
