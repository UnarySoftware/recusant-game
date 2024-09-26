#pragma once

#include <shared/components/snapshot.hpp>
#include <shared/util.hpp>

namespace godot
{
    class CharacterBody3D;
    class Marker3D;
}

namespace recusant
{
    class PlayerSnapshot : public Snapshot
    {
        GD_CLASS(PlayerSnapshot, Snapshot);

    private:
        godot::CharacterBody3D* _player = nullptr;
        godot::Marker3D* _player_head_horizontal = nullptr;
        godot::Marker3D* _player_head_vertical = nullptr;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY_NONCONST(godot::CharacterBody3D*, player);
        IMPL_PROPERTY_NONCONST(godot::Marker3D*, player_head_horizontal);
        IMPL_PROPERTY_NONCONST(godot::Marker3D*, player_head_vertical);

        void _ready() override;
        void _process(double p_delta) override;

        godot::Vector3 provide_position() const override;
        void update_position(godot::Vector3 p_position) override;

        godot::Vector3 provide_rotation() const override;
        void update_rotation(godot::Vector3 p_rotation) override;
    };
}
