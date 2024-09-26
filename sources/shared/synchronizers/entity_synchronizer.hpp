#pragma once

#include <shared/synchronizers/base_synchronizer.hpp>
#include <shared/util.hpp>

namespace godot
{
    class Node3D;
}

namespace recusant
{
    class EntitySynchronizer : public BaseSynchronizer
    {
        GD_CLASS(EntitySynchronizer, BaseSynchronizer);

    private:

        godot::Node3D* _visibility_node;
        godot::NodePath _visibility_node_path;

    protected:

        static void _bind_methods();

    public:

        IMPL_PROPERTY_NONCONST(godot::Node3D*, visibility_node);

        void _ready() override;
        void set_sleep(bool p_value) override;
    };
}
