#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/cylinder_shape3d.hpp>

#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/capsule_mesh.hpp>
#include <godot_cpp/classes/cylinder_mesh.hpp>

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/variant/node_path.hpp>

#include <level/collision_shape_visual_3d.hpp>

using namespace godot;

namespace recusant
{
    void CollisionShapeVisual3D::_bind_methods()
    {
        BIND_METHOD(_on_shape_data_changed);

        BIND_PROPERTY_HINTED(Variant::INT, type, PropertyHint::PROPERTY_HINT_ENUM, "Zone,Clip");

        BIND_PROPERTY_HINTED(Variant::NODE_PATH, manager, PropertyHint::PROPERTY_HINT_NODE_PATH_VALID_TYPES, "CollisionShapeVisualManager");
    }

    void CollisionShapeVisual3D::set_manager(const godot::NodePath& p_manager)
    {
        _manager = p_manager;
        _on_shape_type_changed();
    }

    NodePath CollisionShapeVisual3D::get_manager() const
    {
        return _manager;
    }

    const godot::MeshInstance3D* CollisionShapeVisual3D::get_visualizer_mesh() const
    {
        return _visualizer_mesh;
    }

    void CollisionShapeVisual3D::set_type(const CollisionShapeVisualManager::Type p_type)
    {
        _type = p_type;
        _on_shape_type_changed();
    }

    CollisionShapeVisualManager::Type CollisionShapeVisual3D::get_type() const
    {
        return _type;
    }

    void CollisionShapeVisual3D::_on_shape_type_changed()
    {
        CollisionShapeVisualManager* manager = cast_to<CollisionShapeVisualManager>(get_node_or_null(_manager));

        if (!manager || !_visualizer_mesh || _visualizer_mesh->get_surface_override_material_count() == 0)
        {
            return;
        }

        switch (_type)
        {
        case CollisionShapeVisualManager::Type::Zone:
        {
            _visualizer_mesh->set_surface_override_material(0, manager->get_zone());
            break;
        }
        case CollisionShapeVisualManager::Type::Clip:
        {
            _visualizer_mesh->set_surface_override_material(0, manager->get_clip());
            break;
        }
        default:
        {
            break;
        }
        }
    }

    void CollisionShapeVisual3D::_on_shape_data_changed()
    {
        switch (_current_shape_type)
        {
        case ShapeType::Sphere:
        {
            const SphereShape3D* shape = cast_to<SphereShape3D>(_current_shape);
            SphereMesh* mesh = cast_to<SphereMesh>(_visualizer_mesh->get_mesh().ptr());
            mesh->set_height(shape->get_radius() * 2.0);
            mesh->set_radius(shape->get_radius());
            break;
        }
        case ShapeType::Box:
        {
            const BoxShape3D* shape = cast_to<BoxShape3D>(_current_shape);
            BoxMesh* mesh = cast_to<BoxMesh>(_visualizer_mesh->get_mesh().ptr());
            mesh->set_size(shape->get_size());
            break;
        }
        case ShapeType::Capsule:
        {
            const CapsuleShape3D* shape = cast_to<CapsuleShape3D>(_current_shape);
            CapsuleMesh* mesh = cast_to<CapsuleMesh>(_visualizer_mesh->get_mesh().ptr());
            mesh->set_height(shape->get_height());
            mesh->set_radius(shape->get_radius());
            break;
        }
        case ShapeType::Cylinder:
        {
            const CylinderShape3D* shape = cast_to<CylinderShape3D>(_current_shape);
            CylinderMesh* mesh = cast_to<CylinderMesh>(_visualizer_mesh->get_mesh().ptr());
            mesh->set_top_radius(shape->get_radius());
            mesh->set_bottom_radius(shape->get_radius());
            mesh->set_height(shape->get_height());
            break;
        }
        default:
        {
            break;
        }
        }
    }

    void CollisionShapeVisual3D::_on_shape_asset_changed(const Ref<Shape3D> p_shape)
    {
        Shape3D* shape = p_shape.ptr();

        if (cast_to<SphereShape3D>(shape))
        {
            _current_shape_type = ShapeType::Sphere;
            _visualizer_mesh->set_mesh(Ref<SphereMesh>(memnew(SphereMesh)));
        }
        else if (cast_to<BoxShape3D>(shape))
        {
            _current_shape_type = ShapeType::Box;
            _visualizer_mesh->set_mesh(Ref<BoxMesh>(memnew(BoxMesh)));
        }
        else if (cast_to<CapsuleShape3D>(shape))
        {
            _current_shape_type = ShapeType::Capsule;
            _visualizer_mesh->set_mesh(Ref<CapsuleMesh>(memnew(CapsuleMesh)));
        }
        else if (cast_to<CylinderShape3D>(shape))
        {
            _current_shape_type = ShapeType::Cylinder;
            _visualizer_mesh->set_mesh(Ref<CylinderMesh>(memnew(CylinderMesh)));
        }
        else
        {
            _current_shape_type = ShapeType::None;
            _visualizer_mesh->set_mesh(Ref<Mesh>(nullptr));
            return;
        }

        _current_shape = p_shape.ptr();
        _on_shape_data_changed();
        _on_shape_type_changed();
        p_shape->connect("changed", Callable(this, NAMEOF(_on_shape_data_changed)));
    }

    void CollisionShapeVisual3D::_ready()
    {
        set_process(IS_EDITOR);

        EDITOR_ONLY();

        _timer.instantiate();
        _timer->set(20.0f);

        _current_shape = nullptr;

        int32_t children = get_child_count();

        for (int32_t i = 0; i < children; ++i)
        {
            MeshInstance3D* mesh = cast_to<MeshInstance3D>(get_child(i));
            if (mesh && mesh->has_meta("CCSVisualizer"))
            {
                mesh->queue_free();
            }
        }

        _visualizer_mesh = memnew(MeshInstance3D);
        _visualizer_mesh->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting::SHADOW_CASTING_SETTING_OFF);
        _visualizer_mesh->set_meta("CCSVisualizer", true);

        add_child(_visualizer_mesh, false, Node::INTERNAL_MODE_FRONT);
    }

    void CollisionShapeVisual3D::_process(double p_delta)
    {
        EDITOR_ONLY();

        if (!_timer->update(p_delta))
        {
            return;
        }

        const Ref<Shape3D> shape = get_shape();

        if (shape != _current_shape)
        {
            _on_shape_asset_changed(shape);
        }
    }

}