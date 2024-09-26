#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/marker3d.hpp>
#include <godot_cpp/classes/navigation_agent3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/project_settings.hpp>

#include <dynamic/soldier/soldier_snapshot.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <dynamic/soldier/pathfinding.hpp>

using namespace godot;

namespace recusant
{
    void SoldierPathfinding::_bind_methods()
    {
        BIND_PROPERTY_PATH(NavigationAgent3D, agent);
        BIND_PROPERTY_PATH(Node3D, raycasts_parent);
        BIND_PROPERTY(Variant::FLOAT, movement_speed);
        BIND_PROPERTY(Variant::FLOAT, lerp_factor);
        BIND_METHOD(on_velocity_computed);
    }

    void SoldierPathfinding::set_movement_target(Vector3 p_movement_target)
    {
        _agent->set_target_position(p_movement_target);
    }

    void SoldierPathfinding::on_velocity_computed(godot::Vector3 p_safe_velocity)
    {
        CharacterBody3D* body = _snapshot->get_soldier();
        body->set_velocity(body->get_velocity().move_toward(p_safe_velocity, _lerp_factor));
    }

    void SoldierPathfinding::_ready()
    {
        RUNTIME_ONLY();

        set_process(is_multiplayer_authority());
        set_physics_process(is_multiplayer_authority());

        if (!is_multiplayer_authority())
        {
            return;
        }

        for (int i = 0; i < _raycasts_parent->get_child_count(); ++i)
        {
            _raycasts.push_back(cast_to<RayCast3D>(_raycasts_parent->get_child(i)));
        }

        _snapshot = get_component<SoldierSnapshot>();
        _gravity = ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity");

        //agent->connect("velocity_computed", Callable(this, "on_velocity_computed"));

        _process_block.instantiate();
        _process_block->set(0.33);
    }

    void SoldierPathfinding::_process(double p_delta)
    {
        RUNTIME_ONLY();

        if (!_process_block->update(p_delta))
        {
            return;
        }

        int index = UtilityFunctions::randi_range(0, _raycasts.size() - 1);
        RayCast3D* target = cast_to<RayCast3D>(_raycasts[index]);
        if (!target->is_colliding())
        {
            return;
        }
        set_movement_target(target->get_collision_point());
    }

    void SoldierPathfinding::_physics_process(double p_delta)
    {
        RUNTIME_ONLY();

        if (_agent->is_navigation_finished())
        {
            return;
        }

        auto body = _snapshot->get_soldier();
        Vector3 global_position = body->get_global_position();

        Vector3 next_position = _agent->get_next_path_position();
        Vector3 offset = next_position - global_position;
        
        body->set_global_position(global_position.move_toward(next_position, p_delta * 3.5));

        offset.y = 0;
        if (!offset.is_zero_approx())
        {
            Vector3 rotation = _snapshot->get_rotation();
            _snapshot->get_soldier_head()->look_at(global_position + offset, Vector3(0, 1, 0));
        }


        /*
        Vector3 current_location = transform->get_position();
        Vector3 next_location = agent->get_next_path_position();
        Vector3 new_velocity = next_location - current_location;
        new_velocity.normalize();
        new_velocity *= movement_speed;
        agent->set_velocity(new_velocity);
        */
        _snapshot->get_soldier()->move_and_slide();
    }
}
