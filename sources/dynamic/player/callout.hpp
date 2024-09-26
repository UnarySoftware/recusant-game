#pragma once

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace godot
{
    class RayCast3D;
    class Node3D;
}

namespace recusant
{
    class PlayerCallout : public Component
    {
        GD_CLASS(PlayerCallout, Component);

    private:
        
        godot::RayCast3D* _raycast = nullptr;
        godot::Node3D* _callout = nullptr;

    protected:
        
        static void _bind_methods();

    public:
        
        IMPL_PROPERTY_NONCONST(godot::RayCast3D*, raycast);
        IMPL_PROPERTY_NONCONST(godot::Node3D*, callout);

        void set_new_callout(godot::Vector3 p_position);

        void _ready() override;
        void on_authority_changed() override;
        void _process(double p_delta) override;
    };
}
