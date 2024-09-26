#pragma once

#include <godot_cpp/classes/packed_scene.hpp>
#include <system/system.hpp>
#include <shared/util.hpp>

namespace recusant
{
    class EntitySpawner;

    class LevelManager : public System
    {
        GD_CLASS(LevelManager, System);

    private:

        DEFINE_SINGLETON();

        godot::String _current_level_name;
        godot::Node* _current_level = nullptr;

    protected:
        
        static void _bind_methods();

    public:

        IMPL_PROPERTY(godot::String, current_level_name);
        IMPL_PROPERTY_NONCONST(godot::Node*, current_level);

        IMPL_SINGLETON();

        void change_level(godot::String p_new_level);
        
        void initialize() override;
        void deinitialize() override;

    };
}
