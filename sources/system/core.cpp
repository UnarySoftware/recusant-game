#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <system/logger.hpp>

#include <system/core.hpp>

using namespace godot;

namespace recusant
{
    Core* Core::_singleton = nullptr;

    Core* Core::get_singleton()
    {
        return _singleton;
    }

    void Core::_bind_methods()
    {
        godot::ClassDB::bind_static_method("Core", D_METHOD("get_singleton"), &Core::get_singleton);
    }

    void Core::_ready()
    {
        if (godot::Engine::get_singleton()->is_editor_hint())
        {
            return;
        }

        Util::_initialized = true;

        _singleton = this;

        std::vector<Node*> children;
        Util::get_children_recursive(this, children);

        for (auto child : children)
        {
            String class_name = child->get_class();

            if (class_name == "Core")
            {
                continue;
            }
            else if (class_name == "Logger")
            {
                Log::get_singleton()->set_wrapper(cast_to<Logger>(child));
            }

            System* system = cast_to<System>(child);

            if (system == nullptr)
            {
                continue;
            }
            
            _system_map[class_name] = system;
            _system_vec.push_back(system);
            system->initialize();
        }
    }

    void Core::_exit_tree()
    {
        RUNTIME_ONLY();

        for (int i = _system_vec.size() - 1; i >= 0; i--)
        {
            System* target = _system_vec[i];
            target->deinitialize();
        }

        if (_singleton != nullptr)
        {
            _singleton = nullptr;
        }
    }

    const std::vector<System*> Core::get_systems() const
    {
        return _system_vec;
    }
}