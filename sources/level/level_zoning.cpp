#include <level/zone_node_3d.hpp>

#include <level/level_zoning.hpp>

using namespace godot;

namespace recusant
{
    void LevelZoning::_bind_methods()
    {
        BIND_PROPERTY(Variant::BOOL, zones_visible);
    }

    void LevelZoning::set_zones_visible(const bool p_zones_visible)
    {
        int32_t children = get_child_count();

        for (int32_t i = 0; i < children; ++i)
        {
            ZoneNode3D* zone = cast_to<ZoneNode3D>(get_child(i));
            if (zone)
            {
                if (p_zones_visible)
                {
                    zone->show();
                }
                else
                {
                    zone->hide();
                }
            }
        }

        _zones_visible = p_zones_visible;
    }

    bool LevelZoning::get_zones_visible() const
    {
        return _zones_visible;
    };
}