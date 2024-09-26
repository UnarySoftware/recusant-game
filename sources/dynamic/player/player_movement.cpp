#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/marker3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/audio_listener3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>

#include <dynamic/player/player_movement.hpp>

using namespace godot;

namespace recusant
{
    void PlayerMovement::_bind_methods()
    {
        BIND_PROPERTY(Variant::FLOAT, sensitivity);
        BIND_PROPERTY(Variant::FLOAT, speed);
        BIND_PROPERTY(Variant::FLOAT, acceleration);
        BIND_PROPERTY(Variant::FLOAT, jump_velocity);
        BIND_PROPERTY(Variant::FLOAT, max_speed);
        BIND_PROPERTY(Variant::FLOAT, jump_height);

        BIND_PROPERTY_PATH(CharacterBody3D, body);
        BIND_PROPERTY_PATH(Marker3D, head_horizontal);
        BIND_PROPERTY_PATH(Marker3D, head_vertical);
        BIND_PROPERTY_PATH(AudioListener3D, listener);
        BIND_PROPERTY_PATH(Camera3D, camera);
        BIND_PROPERTY_PATH(CollisionShape3D, shape);
        BIND_PROPERTY_PATH(MeshInstance3D, mesh);
    }

    void PlayerMovement::_ready()
    {
        RUNTIME_ONLY();

        _gravity = ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity");

        Input::get_singleton()->set_mouse_mode(Input::MouseMode::MOUSE_MODE_CAPTURED);
    }

    void PlayerMovement::on_authority_changed()
    {
        set_process_input(is_multiplayer_authority());
        set_physics_process(is_multiplayer_authority());

        if (is_multiplayer_authority())
        {
            _camera->set_current(true);
            _shape->set_disabled(false);
            _listener->make_current();
        }
        else
        {
            _camera->set_current(false);
            _shape->set_disabled(true);
            _listener->clear_current();
        }
    }

    void PlayerMovement::_input(const Ref<InputEvent>& p_event)
    {
        RUNTIME_ONLY();

        if (Input::get_singleton()->get_mouse_mode() != Input::MouseMode::MOUSE_MODE_CAPTURED)
        {
            return;
        }

        if (Input::get_singleton()->is_key_pressed(Key::KEY_ESCAPE))
        {
            Input::get_singleton()->set_mouse_mode(Input::MouseMode::MOUSE_MODE_VISIBLE);
        }

        const InputEventMouseMotion* motion = cast_to<InputEventMouseMotion>(p_event.ptr());
        if (motion != nullptr)
        {
            _head_horizontal->rotate_y(-motion->get_relative().x / _sensitivity);
            _head_vertical->rotate_x(-motion->get_relative().y / _sensitivity);

            Vector3 head_rot = _head_vertical->get_rotation_degrees();
            head_rot.x = Math::clamp(head_rot.x, -89.9f, 89.9f);
            _head_vertical->set_rotation_degrees(head_rot);
        }
    }

    void PlayerMovement::sprint()
    {
        if (Input::get_singleton()->is_action_just_pressed("move_sprint"))
        {
            _speed = _max_speed;
        }
    }

    void PlayerMovement::_physics_process(double p_delta)
    {
        RUNTIME_ONLY();

        if (Input::get_singleton()->get_mouse_mode() != Input::MouseMode::MOUSE_MODE_CAPTURED)
        {
            return;
        }

        Vector3 velocity = _body->get_velocity();

        if (!_body->is_on_floor())
        {
            velocity.y -= _gravity * p_delta;
        }

        if (Input::get_singleton()->is_action_pressed("move_jump") && _body->is_on_floor())
        {
            velocity.y = _jump_velocity;
        }

        Vector2 input_dir = Input::get_singleton()->get_vector("move_left", "move_right", "move_forward", "move_backward");
        input_dir = input_dir.normalized();

        Vector3 direction;

        if (input_dir != Vector2(0.0f, 0.0f))
        {
            direction.x = input_dir.x;
            direction.y = 0.0f;
            direction.z = input_dir.y;
            direction.rotate(Vector3(0.0f, 1.0f, 0.0f), _head_horizontal->get_rotation().y);
        }

        sprint();

        Vector3 motion = velocity;
        velocity = velocity.lerp(direction * _speed, _acceleration * p_delta);
        motion.x = velocity.x;
        motion.z = velocity.z;

        _body->set_velocity(motion);
        _body->move_and_slide();
    }
}
