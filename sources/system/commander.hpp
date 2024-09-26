#pragma once

#include <system/core.hpp>
#include <system/system.hpp>
#include <shared/util.hpp>

#include <godot_cpp/classes/expression.hpp>
#include <godot_cpp/classes/reg_ex.hpp>

namespace recusant
{
    class Commander : public System
    {
        GD_CLASS(Commander, System);

    private:

        DEFINE_SINGLETON();
        
        godot::Ref<godot::Expression> _expression;
        godot::Array _empty_array;
        godot::Variant _empty_variant;
        godot::Ref<godot::RegEx> _regex;

    protected:
        
        static void _bind_methods();

    public:

        IMPL_SINGLETON();

        godot::StringName parse_fail;
        godot::StringName execute_fail;

        Commander()
        {
            parse_fail = "parse_fail";
            execute_fail = "execute_fail";
        }
        
        void initialize() override;
        void deinitialize() override;

        godot::Variant execute(godot::String p_command);

    };
}
