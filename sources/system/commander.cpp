#include <godot_cpp/classes/reg_ex_match.hpp>

#include <system/commander.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(Commander);

    void Commander::_bind_methods()
    {
        BIND_SINGLETON();

        ADD_SIGNAL(MethodInfo("parse_fail", PropertyInfo(Variant::STRING, "error")));
        ADD_SIGNAL(MethodInfo("execute_fail"));
    }

    void Commander::initialize()
    {
        ASSIGN_SINGLETON();
        _empty_array = Array();
        _empty_variant = Variant();
        _expression.instantiate();
        _regex.instantiate();
        _regex->compile("(.*?)\\.(.*?)\\((.*)");
    }

    void Commander::deinitialize()
    {
        UNASSIGN_SINGLETON();
    }

    Variant Commander::execute(godot::String p_command)
    {
        godot::Ref<godot::RegExMatch> match = _regex->search(p_command);

        if (match.is_null())
        {
            emit_signal(parse_fail, "Invalid syntax.");
            return _empty_variant;
        }

        const std::vector<System*> systems = Core::get_singleton()->get_systems();
        System* target_system = nullptr;
        bool found_method = false;

        for (const auto& system : systems)
        {
            if (match->get_string(1) == system->get_console_name())
            {
                target_system = system;
                if (system->get_console_allowed().find(match->get_string(2)) != -1)
                {
                    found_method = true;
                }
            }
        }

        if (target_system == nullptr)
        {
            emit_signal(parse_fail, "Failed to find " + match->get_string(1) + " system");
            return _empty_variant;
        }
        else if (!found_method)
        {
            emit_signal(parse_fail, "Failed to find method " + match->get_string(2) + " for system " + match->get_string(1));
            return _empty_variant;
        }

        String command = match->get_string(2) + "(" + match->get_string(3);

        Error error = _expression->parse(command);
        if (error != OK)
        {
            emit_signal(parse_fail, _expression->get_error_text());
            return _empty_variant;
        }

        Variant result = _expression->execute(_empty_array, target_system, true, false);
        if (_expression->has_execute_failed())
        {
            emit_signal(execute_fail, _expression->get_error_text());
            return _empty_variant;
        }

        return result;
    }
}
