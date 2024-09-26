#pragma once

#include <level/collision_shape_visual_manager.hpp>

#include <godot_cpp/classes/collision_shape3d.hpp>
#include <shared/util.hpp>

#include <shared/units/sleep_block.hpp>

namespace godot
{
    class MeshInstance3D;
}

namespace recusant
{
    class CollisionShapeVisualManager;

    class CollisionShapeVisual3D : public godot::CollisionShape3D
    {
        GD_CLASS(CollisionShapeVisual3D, CollisionShape3D);

    private:

        enum ShapeType
        {
            None = 0,
            Sphere,
            Box,
            Capsule,
            Cylinder
        };

        godot::Shape3D* _current_shape = nullptr;
        ShapeType _current_shape_type = ShapeType::None;
        
        godot::NodePath _manager;
        godot::MeshInstance3D* _visualizer_mesh = nullptr;

        godot::Ref<SleepBlock> _timer;

    protected:

        CollisionShapeVisualManager::Type _type = CollisionShapeVisualManager::Zone;
        static void _bind_methods();

    public:

        IMPL_PROPERTY_NONCONST(godot::MeshInstance3D*, visualizer_mesh);
        
        void set_manager(const godot::NodePath& p_manager);
        godot::NodePath get_manager() const;

        const godot::MeshInstance3D* get_visualizer_mesh() const;
        
        virtual void set_type(const CollisionShapeVisualManager::Type p_type);
        virtual CollisionShapeVisualManager::Type get_type() const;

        void _on_shape_type_changed();
        virtual void _on_shape_data_changed();
        void _on_shape_asset_changed(const godot::Ref<godot::Shape3D> p_shape);

        void _ready() override;
        void _process(double p_delta) override;

    };

}
