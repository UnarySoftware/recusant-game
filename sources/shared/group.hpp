#pragma once

#include <godot_cpp/classes/node.hpp>

namespace recusant
{
    class Group : public godot::Node
    {
        GDCLASS(Group, Node);
    protected:
        static void _bind_methods() {};
    };
}
