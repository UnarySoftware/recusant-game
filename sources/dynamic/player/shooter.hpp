#pragma once

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class PlayerShooter : public Component
    {
        GD_CLASS(PlayerShooter, Component);

    private:
        godot::RayCast3D* _raycast = nullptr;
        godot::Node3D* _decal = nullptr;
        godot::Array _decals;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY_NONCONST(godot::RayCast3D*, raycast);
        IMPL_PROPERTY_NONCONST(godot::Node3D*, decal);

        void _ready() override;
        void on_authority_changed() override;
        void _exit_tree() override;
        void _process(double p_delta) override;
    };
}
