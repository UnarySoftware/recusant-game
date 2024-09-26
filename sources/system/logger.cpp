#include <godot_cpp/variant/utility_functions.hpp>

#include <system/logger.hpp>

using namespace godot;

namespace recusant
{
    Log* Log::_singleton = nullptr;
    Logger* Log::_wrapper = nullptr;
    Logger* Logger::_singleton = nullptr;
    std::string Log::_buffer;

    void Logger::_bind_methods()
    {
        godot::ClassDB::bind_static_method(Logger::get_class_static(), D_METHOD("get_singleton"), &Logger::get_singleton);

        ADD_SIGNAL(MethodInfo("on_print", PropertyInfo(Variant::STRING, "text")));
        ADD_SIGNAL(MethodInfo("on_warning", PropertyInfo(Variant::STRING, "text")));
        ADD_SIGNAL(MethodInfo("on_error", PropertyInfo(Variant::STRING, "text")));
        ADD_SIGNAL(MethodInfo("on_rich", PropertyInfo(Variant::STRING, "text")));

        ClassDB::bind_method(D_METHOD("print"), &Logger::print);
        ClassDB::bind_method(D_METHOD("warning"), &Logger::warning);
        ClassDB::bind_method(D_METHOD("error"), &Logger::error);
        ClassDB::bind_method(D_METHOD("rich"), &Logger::rich);
    }

    void Logger::initialize()
    {
        _singleton = this;
    }

    void Logger::deinitialize()
    {
        if (_singleton != nullptr)
        {
            _singleton = nullptr;
        }
    }

    void Logger::signal(StringName p_signal, const char* p_text)
    {
        emit_signal(p_signal, p_text);
    }

    godot::String Logger::print(String p_text)
    {
        Log::print() << p_text << Log::end();
        return p_text;
    }

    godot::String Logger::warning(String p_text)
    {
        Log::warning() << p_text << Log::end();
        return p_text;
    }

    godot::String Logger::error(String p_text)
    {
        Log::error() << p_text << Log::end();
        return p_text;
    }

    godot::String Logger::rich(String p_text)
    {
        Log::rich() << p_text << Log::end();
        return p_text;
    }

    godot::PackedStringArray Logger::get_console_allowed()
    {
        PackedStringArray arr = { "print", "warning", "error", "rich" };
        return arr;
    }

    void Log::set_wrapper(Logger* p_wrapper)
    {
        _wrapper = p_wrapper;
    }

    Log* Log::get_singleton()
    {
        if (_singleton == nullptr)
        {
            _singleton = memnew(Log);
        }
        return _singleton;
    }

    Log& Log::print()
    {
        get_singleton()->_type = Print;
        return *_singleton;
    }

    Log& Log::warning()
    {
        get_singleton()->_type = Warning;
        return *_singleton;
    }

    Log& Log::error()
    {
        get_singleton()->_type = Error;
        return *_singleton;
    }

    Log& Log::rich()
    {
        get_singleton()->_type = Rich;
        return *_singleton;
    }

    Log& Log::operator<<(const godot::Variant& p_arg)
    {
        _buffer += p_arg.stringify().utf8();
        return *this;
    }

    Log& Log::operator<<(const Log::End& p_arg)
    {
        const char* result = _buffer.c_str();
        switch (_type)
        {
        default:
        case Print:
        {
            godot::UtilityFunctions::print(result);
            if (_wrapper)
            {
                _wrapper->signal(_wrapper->on_print, result);
            }
            break;
        }
        case Warning:
        {
            godot::UtilityFunctions::push_warning(result);
            if (_wrapper)
            {
                _wrapper->signal(_wrapper->on_warning, result);
            }
            break;
        }
        case Error:
        {
            godot::UtilityFunctions::push_error(result);
            if (_wrapper)
            {
                _wrapper->signal(_wrapper->on_error, result);
            }
            break;
        }
        case Rich:
        {
            godot::UtilityFunctions::print_rich(result);
            if (_wrapper)
            {
                _wrapper->signal(_wrapper->on_rich, result);
            }
            break;
        }
        }
        _buffer.clear();

        return *this;
    }

    Log::End Log::end()
    {
        return Log::End::Flush;
    }
}