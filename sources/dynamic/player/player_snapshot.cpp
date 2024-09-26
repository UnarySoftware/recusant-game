#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/marker3d.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include <dynamic/player/player_snapshot.hpp>

using namespace godot;

namespace recusant
{
    void PlayerSnapshot::_bind_methods()
    {
        BIND_PROPERTY_PATH(CharacterBody3D, player);
        BIND_PROPERTY_PATH(Marker3D, player_head_horizontal);
        BIND_PROPERTY_PATH(Marker3D, player_head_vertical);
    }

    void PlayerSnapshot::_ready()
    {
        RUNTIME_ONLY();

        Super::_ready();
    }

    void PlayerSnapshot::_process(double p_delta)
    {
        RUNTIME_ONLY();

        Super::_process(p_delta);
    }

    Vector3 PlayerSnapshot::provide_position() const
    {
        RUNTIME_ONLY(godot::zero::Vector3);

        return _player->get_position();
    }

    void PlayerSnapshot::update_position(Vector3 p_position)
    {
        RUNTIME_ONLY();

        _player->set_position(p_position);
    }

    Vector3 PlayerSnapshot::provide_rotation() const
    {
        RUNTIME_ONLY(godot::zero::Vector3);

        Vector3 rotation;
        rotation.y = _player_head_horizontal->get_rotation().y;
        rotation.x = _player_head_vertical->get_rotation().x;
        return rotation;
    }

    void PlayerSnapshot::update_rotation(Vector3 p_rotation)
    {
        RUNTIME_ONLY();

        Vector3 head_rotation_horizontal;
        head_rotation_horizontal.y = p_rotation.y;
        _player_head_horizontal->set_rotation(head_rotation_horizontal);
        
        Vector3 head_rotation_vertical;
        head_rotation_vertical.x = p_rotation.x;
        _player_head_vertical->set_rotation(head_rotation_vertical);
    }
}
