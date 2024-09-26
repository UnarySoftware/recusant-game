#pragma once

#include <godot_cpp/classes/area3d.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class LevelZoning : public godot::Area3D
    {
        GD_CLASS(LevelZoning, Area3D);

    private:

        bool _zones_visible = true;

    protected:

        static void _bind_methods();

    public:

        void set_zones_visible(const bool p_zones_visible);
        bool get_zones_visible() const;

        void _ready() override
        {
            set_monitoring(false);
            set_monitorable(false);
        }

    };

}
