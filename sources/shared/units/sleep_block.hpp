#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

namespace recusant
{

    class SleepBlock : public godot::RefCounted
    {
        GD_CLASS(SleepBlock, RefCounted);

    private:

        // Used to evenly disperse sleep block processing through different frames
        // Usefull for things like level entities that all get enabled on the same frame
        static const double _offset_step;
        static const double _max_offset;
        static double _current_step;

        enum class Type
        {
            Range,
            Singular,
            Disabled
        } _type;

        double _span = 0.0;
        double _timeout = 0.0;
        double _timer = 0.0;
        bool _sleeping = false;

    protected:

        static void _bind_methods();

    public:

        godot::StringName on_active;
        godot::StringName on_sleep;

        SleepBlock()
        {
            on_active = "on_active";
            on_sleep = "on_active";
        }

        // p_tps - how many times per second should the sleeping function execute
        // 20.0f - 20 times per second
        // 1.0f - 1 time per second
        // 0.5f - 1 time every 2 seconds
        // 0.25f - 1 time every 4 seconds
        void set(double p_tps);
        // p_span - for how many seconds should the function sleep
        // p_timeout - for how many seconds should the function run
        // For example, if we want to have a trigger that would enable itself every 3 second, and
        // would be working for 0.5 seconds, params would be set(3.0, 0.5);
        void set(double p_span, double p_timeout);

        bool update(double p_delta);
    };
}
