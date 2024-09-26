#pragma once

#include <shared/components/snapshot.hpp>
#include <shared/util.hpp>

namespace godot
{
    class Marker3D;
}

namespace recusant
{
    class SoldierSnapshot : public Snapshot
    {
        GD_CLASS(SoldierSnapshot, Snapshot);

    private:
        godot::CharacterBody3D* _soldier = nullptr;
        godot::Marker3D* _soldier_head = nullptr;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY_NONCONST(godot::CharacterBody3D*, soldier);
        IMPL_PROPERTY_NONCONST(godot::Marker3D*, soldier_head);

        void _ready() override;
        void _process(double p_delta) override;

        godot::Vector3 provide_position() const override;
        void update_position(godot::Vector3 p_position) override;

        godot::Vector3 provide_rotation() const override;
        void update_rotation(godot::Vector3 p_rotation) override;
    };
}
