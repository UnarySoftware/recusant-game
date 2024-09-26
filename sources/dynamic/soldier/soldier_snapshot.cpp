#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/marker3d.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include <dynamic/soldier/soldier_snapshot.hpp>

using namespace godot;

namespace recusant
{
    void SoldierSnapshot::_bind_methods()
    {
        BIND_PROPERTY_PATH(CharacterBody3D, soldier);
        BIND_PROPERTY_PATH(Marker3D, soldier_head);
    }

    void SoldierSnapshot::_ready()
    {
        RUNTIME_ONLY();

        Super::_ready();
    }

    void SoldierSnapshot::_process(double p_delta)
    {
        RUNTIME_ONLY();

        Super::_process(p_delta);
    }

    Vector3 SoldierSnapshot::provide_position() const
    {
        RUNTIME_ONLY(godot::zero::Vector3);

        return _soldier->get_position();
    }

    void SoldierSnapshot::update_position(Vector3 p_position)
    {
        RUNTIME_ONLY();

        _soldier->set_position(p_position);
    }

    Vector3 SoldierSnapshot::provide_rotation() const
    {
        RUNTIME_ONLY(godot::zero::Vector3);

        /*
        
        Vector3 rotation;
        rotation.x = soldier_head->get_rotation().x;
        rotation.y = soldier->get_rotation().y;

        */
        return godot::zero::Vector3;
    }

    void SoldierSnapshot::update_rotation(Vector3 p_rotation)
    {
        RUNTIME_ONLY();

        /*

        Vector3 head_rotation;
        head_rotation.x = p_rotation.x;
        soldier_head->set_rotation(head_rotation);

        Vector3 soldier_rotation;
        soldier_rotation.y = p_rotation.y;
        soldier->set_rotation(soldier_rotation);

        */
    }
}
