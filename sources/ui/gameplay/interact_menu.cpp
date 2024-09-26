#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <system/commander.hpp>

#include <ui/gameplay/interact_menu.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(InteractMenu);

    void InteractMenu::_bind_methods()
    {
        BIND_SINGLETON();

        BIND_PROPERTY_PATH(VBoxContainer, root);
        BIND_PROPERTY_PATH(Label, option);
        ADD_SIGNAL(MethodInfo("on_interact", PropertyInfo(Variant::STRING, "option")));
    }
    
    void InteractMenu::_select(int p_index)
    {
        _selected_option = p_index;
        _root->get_child(_selected_option)->get_node<ColorRect>("background")->set_modulate(Color(0.65f, 0.65f, 0.65f));
    }

    void InteractMenu::_select_higher()
    {
        _root->get_child(_selected_option)->get_node<ColorRect>("background")->set_modulate(Color(1.0f, 1.0f, 1.0f));
        _selected_option--;
        if (_selected_option < 1)
        {
            _selected_option = _root->get_child_count() - 1;
        }
        _select(_selected_option);
    }

    void InteractMenu::_select_lower()
    {
        _root->get_child(_selected_option)->get_node<ColorRect>("background")->set_modulate(Color(1.0f, 1.0f, 1.0f));
        _selected_option++;
        if (_selected_option >= _root->get_child_count())
        {
            _selected_option = 1;
        }
        _select(_selected_option);
    }

    void InteractMenu::initialize()
    {
        ASSIGN_SINGLETON();

        _option->set_visible(false);
    }

    void InteractMenu::deinitialize()
    {
        UNASSIGN_SINGLETON();
    }

    void InteractMenu::_process(double p_delta)
    {
        RUNTIME_ONLY();

        if (_root->get_child_count() == 1)
        {
            return;
        }

        if (Input::get_singleton()->is_action_just_released("ui_scroll_up"))
        {
            _select_higher();
        }
        else if (Input::get_singleton()->is_action_just_released("ui_scroll_down"))
        {
            _select_lower();
        }

        if (Input::get_singleton()->is_action_just_pressed("ui_interact"))
        {
            Label* target = cast_to<Label>(_root->get_child(_selected_option));
            if (target != nullptr)
            {
                emit_signal(on_interact, target->get_text());
            }
        }
    }

    void InteractMenu::show(const HashMap<String, Callable>& p_input)
    {
        hide();

        for (const auto& target : p_input)
        {
            auto duplicate_option = _option->duplicate();
            Label* new_label = cast_to<Label>(duplicate_option);
            new_label->set_visible(true);
            new_label->set_text(target.key);
            _root->add_child(new_label);
        }

        _select(1);
    }

    void InteractMenu::hide()
    {
        for (int i = 0; i < _root->get_child_count(); ++i)
        {
            auto child = _root->get_child(i);
            if (child != _option)
            {
                _root->get_child(i)->queue_free();
            }
        }
    }
}
