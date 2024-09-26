#pragma once

#include <system/system.hpp>

namespace recusant
{
    class Logger : public System
    {
        GDCLASS(Logger, System);

    private:

        static Logger* _singleton;

        void _init_sinks();

    protected:

        static void _bind_methods();

    public:

        static Logger* get_singleton()
        {
            return godot::Object::cast_to<Logger>(_singleton);
        };

        godot::StringName on_print;
        godot::StringName on_warning;
        godot::StringName on_error;
        godot::StringName on_rich;

        Logger()
        {
            on_print = "on_print";
            on_warning = "on_warning";
            on_error = "on_error";
            on_rich = "on_rich";
        }

        void initialize() override;
        void deinitialize() override;

        void signal(godot::StringName p_signal, const char* p_text);

        godot::String print(godot::String p_text);
        godot::String warning(godot::String p_text);
        godot::String error(godot::String p_text);
        godot::String rich(godot::String p_text);

        godot::PackedStringArray get_console_allowed() override;
    };

    class Log
    {

    public:

        enum Type
        {
            Print,
            Warning,
            Error,
            Rich
        };

        enum End { Flush };

    private:

        static Log* _singleton;
        static Logger* _wrapper;

        Type _type;
        static std::string _buffer;

    public:

        static void set_wrapper(Logger* p_wrapper);

        static Log* get_singleton();

        static Log& print();
        static Log& warning();
        static Log& error();
        static Log& rich();

        Log& operator<<(const godot::Variant& p_arg);
        Log& operator<<(const Log::End& p_arg);
        static Log::End end();
    };
}
