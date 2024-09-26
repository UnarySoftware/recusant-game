#pragma once

#include <shared/components/interactable.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class DoorInteractable : public Interactable
    {
        GD_CLASS(DoorInteractable, Interactable);

    public:
        enum OpenDirection
        {
            None,
            Forward,
            Backward
        };

    private:

        OpenDirection _direction = OpenDirection::None;
        godot::Node3D* _hinge = nullptr;
        godot::Vector3 _rotation;
        float _weight = 0.0f;
        float _speed = 2.0f;
        float _speed_up = 0.2f;

    protected:

        static void _bind_methods();

    public:
        
        IMPL_PROPERTY(float, speed);
        IMPL_PROPERTY(float, speed_up);

        IMPL_PROPERTY_NONCONST(godot::Node3D*, hinge);

        OpenDirection get_direction();
        void set_direction(OpenDirection p_direction);
        void send_direction(OpenDirection p_direction);

        void _ready() override;
        void _process(double p_delta) override;

        void try_open(Entity* p_caller);
    };
}

VARIANT_ENUM_CAST(recusant::DoorInteractable::OpenDirection)