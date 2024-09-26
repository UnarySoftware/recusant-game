#pragma once

#include <unordered_map>

#include <system/system.hpp>
#include <shared/util.hpp>

namespace recusant
{

    class Core : public godot::Node
    {
        GDCLASS(Core, Node);

    private:

        static Core* _singleton;

        std::vector<System*> _system_vec;
        std::unordered_map<godot::String, System*, godot::hash::String> _system_map;

    protected:
        static void _bind_methods();

    public:

        const std::vector<System*> get_systems() const;
        
        static Core* get_singleton();

        void _ready() override;
        void _exit_tree() override;
    };
}
