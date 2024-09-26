#pragma once

#include <godot_cpp/classes/multiplayer_synchronizer.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class Entity;

    class BaseSynchronizer : public godot::MultiplayerSynchronizer
    {
        GD_CLASS(BaseSynchronizer, MultiplayerSynchronizer);

    private:
        
        Entity* _entity = nullptr;

    protected:

        float _on_change_tps = MAX_TPS;
        float _always_tps = MAX_TPS;
        
        static void _bind_methods();

        Entity* get_entity();

    public:

        godot::StringName on_change_tps_changed;
        godot::StringName always_tps_changed;

        BaseSynchronizer()
        {
            on_change_tps_changed = "on_change_tps_changed";
            always_tps_changed = "always_tps_changed";
        }

        IMPL_PROPERTY(float, on_change_tps);
        IMPL_PROPERTY(float, always_tps);

        virtual void _ready() override;
        virtual void set_sleep(bool p_value) {};

    };
}
