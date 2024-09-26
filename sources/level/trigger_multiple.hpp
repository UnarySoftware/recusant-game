#pragma once

#include <shared/components/component.hpp>
#include <shared/units/sleep_block.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class TriggerMultiple : public Component
    {
        GD_CLASS(TriggerMultiple, Component);

    private:

        godot::Ref<SleepBlock> _process_sleep;
        godot::Area3D* _trigger = nullptr;
        float _sleep_seconds = 1.0f;
        float _active_seconds = 1.0f;

    protected:

        static void _bind_methods();

    public:

        IMPL_PROPERTY_NONCONST(godot::Area3D*, trigger);
        IMPL_PROPERTY(float, sleep_seconds);
        IMPL_PROPERTY(float, active_seconds);

        void _ready() override;

        void on_process_active();
        void on_process_sleep();

        void _process(double p_delta) override;
    };
}