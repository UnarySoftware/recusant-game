#pragma once

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class DoorPathfinding : public Component
    {
        GD_CLASS(DoorPathfinding, Component);

    private:

        godot::CollisionShape3D* _collider = nullptr;

    protected:

        static void _bind_methods();

    public:
        
        IMPL_PROPERTY_NONCONST(godot::CollisionShape3D*, collider);

        void _ready() override;
    };
}
