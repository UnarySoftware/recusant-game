#pragma once

#include <shared/components/component.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class PlayerLabel : public Component
    {
        GD_CLASS(PlayerLabel, Component);

    private:
        godot::Label3D* _label = nullptr;

    protected:
        static void _bind_methods();

    public:
        IMPL_PROPERTY_NONCONST(godot::Label3D*, label);

        void _ready() override;
        void on_authority_changed() override;
    };
}
