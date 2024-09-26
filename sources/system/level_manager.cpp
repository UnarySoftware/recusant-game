#include <godot_cpp/classes/resource_loader.hpp>
#include <system/logger.hpp>

#include <system/level_manager.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(LevelManager);

    void LevelManager::_bind_methods()
    {
        BIND_SINGLETON();
    }

    void LevelManager::change_level(String p_new_level)
    {
        if (!is_multiplayer_authority())
        {
            Log::warning() << "Tried calling change_level on LevelManager when we are not owning it!" << Log::end();
            return;
        }

        auto loader = ResourceLoader::get_singleton();

        String target_path = "res://levels/" + p_new_level + ".tscn";

        if (!loader->exists(target_path, "tscn"))
        {
            Log::warning() << "Tried loading non-existent level " << p_new_level << "! Skipping call! " << Log::end();
            return;
        }

        Error err = loader->load_threaded_request(target_path, "tscn", true, ResourceLoader::CACHE_MODE_REUSE);
        
    }

    void LevelManager::initialize()
    {
        ASSIGN_SINGLETON();

        
    }

    void LevelManager::deinitialize()
    {
        UNASSIGN_SINGLETON();
    }
}
