#pragma once

#include <level/collision_shape_visual_3d.hpp>

#include <shared/units/sleep_block.hpp>
#include <shared/util.hpp>

namespace godot
{
    class Label3D;
}

namespace recusant
{
    class LevelZoning;

    class ZoneNode3D : public CollisionShapeVisual3D
    {
        GD_CLASS(ZoneNode3D, CollisionShapeVisual3D);

    private:

        godot::String _collisions;
        godot::Vector3 _previous_position;
        godot::Label3D* _label = nullptr;
        godot::String _previous_name;
        godot::Ref<SleepBlock> _timer;

    protected:

        static void _bind_methods();

    public:

        IMPL_PROPERTY(godot::String, collisions);

        void set_type(const CollisionShapeVisualManager::Type p_type) override;
        CollisionShapeVisualManager::Type get_type() const override;

        void _on_shape_data_changed() override;
        
        void _ready() override;
        void _process(double p_delta) override;
        void show();
        void hide();

    };

}
