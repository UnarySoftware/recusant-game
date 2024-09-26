#pragma once

#include <shared/components/component.hpp>
#include <shared/util.hpp>

#include <shared/units/motion_generator.hpp>
#include <shared/units/sleep_block.hpp>

namespace recusant
{

    class Snapshot : public Component
    {
        GD_CLASS(Snapshot, Component);

    private:
        
        godot::Ref<MotionGenerator> _generator;

        godot::Vector3 _position;
        godot::Vector3 _rotation;
        float _last_send = 0.0f;
        
    protected:
        
        static void _bind_methods();

    public:

        void on_synchronized();
        
        virtual void _ready() override;
        virtual void _process(double p_delta) override;

        void set_last_send(const float p_last_send);
        // Warning! Resets _last_send on access!
        // Use get_last_send_without_reset() to prevent this behaviour
        float get_last_send();

        float get_last_send_without_reset();

        IMPL_PROPERTY(godot::Vector3, position);
        IMPL_PROPERTY(godot::Vector3, rotation);

        virtual void update_position(const godot::Vector3 p_position);
        virtual godot::Vector3 provide_position() const;

        virtual void update_rotation(const godot::Vector3 p_rotation);
        virtual godot::Vector3 provide_rotation() const;
    };
}
