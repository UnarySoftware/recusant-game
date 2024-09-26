#include <godot_cpp/classes/collision_shape3d.hpp>

#include <level/door/pathfinding.hpp>

using namespace godot;

namespace recusant
{
    void DoorPathfinding::_bind_methods()
    {
        BIND_PROPERTY_PATH(CollisionShape3D, collider);
    }

    void DoorPathfinding::_ready()
    {
        RUNTIME_ONLY();

        _collider->set_disabled(false);
    }
}
