#pragma once

#include <system/system.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class LaunchArgs : public System
    {
        GD_CLASS(LaunchArgs, System);

    private:

        DEFINE_SINGLETON();
        
        godot::String _target_host;
        bool _is_host;
        bool _steam;
        godot::String _target_level;

        godot::String _get_next_arg(int p_current_index, const godot::PackedStringArray &p_args);

    protected:
        
        static void _bind_methods();

    public:

        IMPL_SINGLETON();
        
        const godot::String get_host();
        bool is_host();
        bool is_steam();
        const godot::String get_target_level();

        void initialize() override;
        void deinitialize() override;

    };
}
