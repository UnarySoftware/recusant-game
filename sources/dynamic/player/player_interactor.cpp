
#include <godot_cpp/classes/ray_cast3d.hpp>

#include <dynamic/player/player_snapshot.hpp>
#include <shared/components/interactable.hpp>
#include <system/ui_root.hpp>
#include <ui/gameplay/interact_menu.hpp>

#include <dynamic/player/player_interactor.hpp>

using namespace godot;

namespace recusant
{
    void PlayerInteractor::_bind_methods()
    {
        BIND_PROPERTY_PATH(RayCast3D, raycast);

        BIND_METHOD(on_interact);
    }

    void PlayerInteractor::on_interact(godot::String p_option)
    {
        RUNTIME_ONLY();

        if (_collision_target == nullptr)
        {
            return;
        }

        auto options = _collision_target->get_options();

        if (!options.has(p_option))
        {
            return;
        }

        Entity* entity = get_entity();
        options[p_option].call(entity);
    }

    void PlayerInteractor::_ready()
    {
        RUNTIME_ONLY();

        // TODO Move this to an authority changing place
        _interact_menu = InteractMenu::get_singleton();
        _interact_menu->connect("on_interact", Callable(this, "on_interact"));

        _snapshot = get_component<PlayerSnapshot>();

        _process_sleep.instantiate();
        _process_sleep->set(20.0);
    }

    void PlayerInteractor::on_authority_changed()
    {
        set_process(is_multiplayer_authority());
    }

    void PlayerInteractor::_process(double p_delta)
    {
        RUNTIME_ONLY();

        if (!_process_sleep->update(p_delta))
        {
            return;
        }

        if (_raycast->is_colliding() && _raycast->get_collision_point().distance_to(_snapshot->get_position()) < 5.0f)
        {
            Entity* target = Util::get_entity(cast_to<Node>(_raycast->get_collider()));
            Interactable* interactable = nullptr;
            if (target != nullptr)
            {
                interactable = target->get_component<Interactable>();
            }

            if (interactable == nullptr && _collision_target != nullptr)
            {
                _collision_target = nullptr;
                _interact_menu->hide();
            }
            else if (interactable != _collision_target)
            {
                _collision_target = interactable;
                _interact_menu->show(interactable->get_options());
            }
        }
        else
        {
            if (_collision_target != nullptr)
            {
                _interact_menu->hide();
                _collision_target = nullptr;
            }
        }
    }
}
