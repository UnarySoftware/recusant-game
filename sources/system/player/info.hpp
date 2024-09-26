#pragma once

#include <system/system.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class Info : public System
    {
        GD_CLASS(Info, System);

    private:

        DEFINE_SINGLETON();
        
        godot::Dictionary _player_info;

    protected:
        static void _bind_methods();

    public:

        IMPL_SINGLETON();

        void _ready() override;
        void send_player_info(godot::Dictionary p_player_info) {};
    };
}
