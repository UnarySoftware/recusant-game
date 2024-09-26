#include <shared/entity_spawner.hpp>

using namespace godot;

namespace recusant
{
    void EntitySpawner::_bind_methods()
    {
        BIND_PROPERTY(Variant::STRING, directory);
    }

    void EntitySpawner::_ready()
    {
        RUNTIME_ONLY();

        PackedStringArray files = Util::get_files_recursively(_directory, "tscn");

        for (const auto& file : files)
        {
            add_spawnable_scene(file);
        }
    }
}
