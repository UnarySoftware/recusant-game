#pragma once

#include <ui/ui_component.hpp>
#include <shared/util.hpp>

namespace godot
{
    class LineEdit;
    class VBoxContainer;
}

namespace recusant
{
    class Chat : public UiComponent
    {
        GD_CLASS(Chat, UiComponent);

    private:

        DEFINE_SINGLETON();

        godot::VBoxContainer* _root = nullptr;
        godot::VBoxContainer* _chat_entries = nullptr;
        godot::LineEdit* _input_field = nullptr;
        int _max_message_count = 8;

    protected:

        static void _bind_methods();

    public:

        IMPL_SINGLETON();

        IMPL_PROPERTY_NONCONST(godot::VBoxContainer*, root);
        IMPL_PROPERTY_NONCONST(godot::VBoxContainer*, chat_entries);
        IMPL_PROPERTY_NONCONST(godot::LineEdit*, input_field);
        IMPL_PROPERTY(int, max_message_count);

        void on_parse_fail(godot::String p_error);
        void on_execute_fail(godot::String p_error);

        void player_joined(int p_id);
        void player_left(int p_id);

        void initialize() override;
        void deinitialize() override;

        void input_message(godot::String p_message);
        void print_message(godot::String p_message);
        void send_message(godot::String p_message);
    };
}
