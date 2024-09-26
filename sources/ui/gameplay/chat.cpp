#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <system/commander.hpp>

#include <ui/gameplay/chat.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(Chat);

    void Chat::_bind_methods()
    {
        BIND_SINGLETON();

        BIND_PROPERTY_PATH(VBoxContainer, root);
        BIND_PROPERTY_PATH(VBoxContainer, chat_entries);
        BIND_PROPERTY_PATH(LineEdit, input_field);
        BIND_PROPERTY(Variant::INT, max_message_count);

        BIND_METHOD(on_parse_fail);
        BIND_METHOD(on_execute_fail);

        BIND_METHOD(player_joined);
        BIND_METHOD(player_left);

        BIND_METHOD(input_message);
        BIND_METHOD(print_message);
        BIND_METHOD(send_message);
    }

    void Chat::initialize()
    {
        ASSIGN_SINGLETON();

        Commander::get_singleton()->connect("parse_fail", Callable(this, "on_parse_fail"));
        Commander::get_singleton()->connect("execute_fail", Callable(this, "on_execute_fail"));

        BIND_RPC(send_message, MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TransferMode::TRANSFER_MODE_RELIABLE, true);

        _input_field->connect("text_submitted", Callable(this, "input_message"));

        get_multiplayer()->connect("peer_connected", Callable(this, "player_joined"));
        get_multiplayer()->connect("peer_disconnected", Callable(this, "player_left"));
    }

    void Chat::deinitialize()
    {
        UNASSIGN_SINGLETON();
    }

    void Chat::on_parse_fail(godot::String p_error)
    {
        print_message("Failed to parse last expression with error: " + p_error);
    }

    void Chat::on_execute_fail(godot::String p_error)
    {
        print_message("Failed to execute last expression. Error: " + p_error);
    }

    void Chat::player_joined(int p_id)
    {
        print_message(String::num_int64(p_id) + " has joined the game");
    }

    void Chat::player_left(int p_id)
    {
        print_message(String::num_int64(p_id) + " has left the game");
    }

    void Chat::input_message(godot::String p_message)
    {
        RUNTIME_ONLY();

        _input_field->clear();
        _input_field->release_focus();
        Input::get_singleton()->set_mouse_mode(Input::MouseMode::MOUSE_MODE_CAPTURED);

        if (p_message.begins_with("/"))
        {
            print_message(p_message);
            Variant result = Commander::get_singleton()->execute(p_message.right(-1));
            Variant::Type type = result.get_type();
            if (type != Variant::Type::OBJECT && type != Variant::Type::NIL)
            {
                String command_message = ">" + result.stringify();
                print_message(command_message);
            }
        }
        else
        {
            RPC_EVERYONE(NAMEOF(send_message), p_message);
        }
    }

    void Chat::print_message(godot::String p_message)
    {
        RUNTIME_ONLY();

        if (_chat_entries->get_child_count() > _max_message_count)
        {
            _chat_entries->get_child(0)->queue_free();
        }

        Label *new_message = memnew(Label);
        new_message->set("theme_override_colors/font_outline_color", Color(0.0f, 0.0f, 0.0f, 1.0f));
        new_message->set("theme_override_constants/outline_size", 2);
        new_message->set_text(p_message);
        _chat_entries->add_child(new_message);
    }

    void Chat::send_message(godot::String p_message)
    {
        RUNTIME_ONLY();

        print_message(String::num_int64(get_multiplayer()->get_remote_sender_id()) + ": " + p_message);
    }
}
