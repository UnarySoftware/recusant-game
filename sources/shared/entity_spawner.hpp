#pragma once

#include <godot_cpp/classes/multiplayer_spawner.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class EntitySpawner : public godot::MultiplayerSpawner
    {
        GD_CLASS(EntitySpawner, MultiplayerSpawner);

    private:

        godot::String _directory;

    protected:

        static void _bind_methods();

    public:

        IMPL_PROPERTY(godot::String, directory);
        void _ready() override;

    };
}
