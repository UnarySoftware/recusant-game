#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/decal.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/input.hpp>

#include <dynamic/player/shooter.hpp>

using namespace godot;

namespace recusant
{
    void PlayerShooter::_bind_methods()
    {
        BIND_PROPERTY_PATH(RayCast3D, raycast);
        BIND_PROPERTY_PATH(Node3D, decal);
    }

    void PlayerShooter::_ready()
    {
        RUNTIME_ONLY();

        _decal->set_visible(false);
    }

    void PlayerShooter::on_authority_changed()
    {
        set_process(is_multiplayer_authority());
    }

    void PlayerShooter::_exit_tree()
    {
        RUNTIME_ONLY();
    }

    void PlayerShooter::_process(double p_delta)
    {
        RUNTIME_ONLY();

        if (Input::get_singleton()->is_action_just_pressed("shoot") && _raycast->is_colliding())
        {
            if (_decals.size() >= 4)
            {
                Node3D* target = cast_to<Node3D>(_decals[0]);
                if (Util::is_instance_valid(target))
                {
                    target->queue_free();
                }
                _decals.pop_front();
            }

            Node3D* collider = cast_to<Node3D>(_raycast->get_collider());
            Node3D* new_decal = cast_to<Node3D>(_decal->duplicate());
            new_decal->set_visible(true);
            new_decal->set_disable_scale(true);
            collider->add_child(new_decal);
            new_decal->set_global_position(_raycast->get_collision_point());
            Vector3 target = _raycast->get_collision_point() + _raycast->get_collision_normal();

            if (!_raycast->get_collision_point().is_equal_approx(target))
            {
                if (Math::abs(_raycast->get_collision_normal().y) > 0.99f)
                {
                    new_decal->look_at(target, Vector3(0.0f, 0.0f, 1.0f));
                }
                else
                {
                    new_decal->look_at(target);
                }
            }

            new_decal->rotate_object_local(Vector3(1.0f, 0.0f, 0.0f), -Math_PI / 2.0f);
            _decals.push_back(new_decal);
        }
    }
}
