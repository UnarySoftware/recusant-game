#pragma once

#include <system/system.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class ZoneManager : public System
    {
        GD_CLASS(ZoneManager, System);

    private:

        DEFINE_SINGLETON();

        struct Zone
        {
            godot::AABB aabb;
        };

        std::vector<Zone> _zones;

    protected:
        static void _bind_methods();

    public:

        IMPL_SINGLETON();

        void initialize() override;
        void deinitialize() override;
    };
}
