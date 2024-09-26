#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>

namespace recusant
{
    class System : public godot::Node
    {
        GDCLASS(System, Node);

    public:

        static void _bind_methods();

        virtual void initialize()
        {

        }

        virtual void deinitialize()
        {

        }

        virtual godot::String get_console_name()
        {
            return get_class();
        }

        virtual godot::PackedStringArray get_console_allowed()
        {
            godot::PackedStringArray arr;
            return arr;
        }
    };
}
