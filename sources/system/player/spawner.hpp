#pragma once

#include <godot_cpp/classes/packed_scene.hpp>
#include <system/system.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class PlayerSpawner : public System
    {
        GD_CLASS(PlayerSpawner, System);

    private:

        DEFINE_SINGLETON();
        
        godot::Node* _players = nullptr;
        godot::Ref<godot::PackedScene> _player_scene;

    protected:

        static void _bind_methods();

    public:

        IMPL_SINGLETON();

        IMPL_PROPERTY_NONCONST(godot::Node*, players);
        IMPL_PROPERTY(godot::Ref<godot::PackedScene>, player_scene);

        void spawn_host();
        void initialize() override;
        void deinitialize() override;

        void create_player(int p_id);
        void destroy_player(int id);
    };
}
