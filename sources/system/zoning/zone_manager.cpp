#include <godot_cpp/variant/packed_string_array.hpp>

#include <system/zoning/zone_manager.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(ZoneManager);

    void ZoneManager::_bind_methods()
    {
        BIND_SINGLETON();
    }

    void ZoneManager::initialize()
    {
        ASSIGN_SINGLETON();
    }

    void ZoneManager::deinitialize()
    {
        UNASSIGN_SINGLETON();
    }
}