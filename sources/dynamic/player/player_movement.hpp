#pragma once

#include <godot_cpp/classes/input_event.hpp>

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace godot
{
    class CharacterBody3D;
    class Marker3D;
    class AudioListener3D;
}

namespace recusant
{
    class PlayerMovement : public Component
    {
        GD_CLASS(PlayerMovement, Component);

    protected:
        static void _bind_methods();

    private:
        float _sensitivity = 700.0f;
        float _speed = 6.0f;
        float _acceleration = 10.0f;
        float _jump_velocity = 4.5f;
        float _max_speed = 8.0f;
        float _jump_height = 4.0f;

        float _gravity = 0.0f;

        godot::CharacterBody3D* _body = nullptr;
        godot::Marker3D* _head_horizontal = nullptr;
        godot::Marker3D* _head_vertical = nullptr;
        godot::Camera3D* _camera = nullptr;
        godot::AudioListener3D* _listener = nullptr;
        godot::CollisionShape3D* _shape = nullptr;
        godot::MeshInstance3D* _mesh = nullptr;

    public:
        IMPL_PROPERTY(float, sensitivity);
        IMPL_PROPERTY(float, speed);
        IMPL_PROPERTY(float, acceleration);
        IMPL_PROPERTY(float, jump_velocity);
        IMPL_PROPERTY(float, max_speed);
        IMPL_PROPERTY(float, jump_height);

        IMPL_PROPERTY_NONCONST(godot::CharacterBody3D*, body);
        IMPL_PROPERTY_NONCONST(godot::Marker3D*, head_horizontal);
        IMPL_PROPERTY_NONCONST(godot::Marker3D*, head_vertical);
        IMPL_PROPERTY_NONCONST(godot::Camera3D*, camera);
        IMPL_PROPERTY_NONCONST(godot::AudioListener3D*, listener);
        IMPL_PROPERTY_NONCONST(godot::CollisionShape3D*, shape);
        IMPL_PROPERTY_NONCONST(godot::MeshInstance3D*, mesh);

        void _ready() override;
        void on_authority_changed() override;
        void _input(const godot::Ref<godot::InputEvent>& p_event) override;
        void sprint();
        void _physics_process(double p_delta) override;
    };
}
