#include <level/level_zoning.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/label3d.hpp>
#include <system/logger.hpp>

#include <level/zone_node_3d.hpp>

using namespace godot;

namespace recusant
{
    void ZoneNode3D::_bind_methods()
    {
        BIND_METHOD(_on_shape_data_changed);

        BIND_PROPERTY_HINTED_USAGE(Variant::STRING, collisions, PROPERTY_HINT_MULTILINE_TEXT, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_READ_ONLY);
    }

    void ZoneNode3D::set_type(const CollisionShapeVisualManager::Type p_type)
    {
        _type = CollisionShapeVisualManager::Type::Zone;
    }

    CollisionShapeVisualManager::Type ZoneNode3D::get_type() const
    {
        return CollisionShapeVisualManager::Type::Zone;
    }

    void ZoneNode3D::_on_shape_data_changed()
    {
        CollisionShapeVisual3D::_on_shape_data_changed();

        _collisions = "";

        Node* parent = get_parent();

        if (!parent)
        {
            return;
        }

        LevelZoning* level_zoner = cast_to<LevelZoning>(parent);

        if (!level_zoner)
        {
            return;
        }

        const Ref<Shape3D> shape = get_shape();
        const BoxShape3D* box = cast_to<BoxShape3D>(shape.ptr());

        if (!box)
        {
            return;
        }

        int32_t children = level_zoner->get_child_count();

        Vector3 box_size = box->get_size();
        Vector3 origin = get_transform().origin;
        AABB our_box = AABB(origin - (box_size / 2.0f), box_size);

        if (our_box.size.x * our_box.size.z < 22500.0f) // 150 x 150
        {
            Log::warning() << get_name() << " zone is too small, this might result in weird pop-ins!" << Log::end();
        }

        for (int32_t i = 0; i < children; ++i)
        {
            ZoneNode3D* zone = cast_to<ZoneNode3D>(level_zoner->get_child(i));
            if (zone && zone != this)
            {
                const Ref<Shape3D> zone_shape = zone->get_shape();
                const BoxShape3D* zone_box = cast_to<BoxShape3D>(zone_shape.ptr());

                if (!zone_box)
                {
                    continue;
                }

                Vector3 target_box_size = zone_box->get_size();
                Vector3 target_origin = zone->get_transform().origin;
                AABB target_box = AABB(target_origin - (target_box_size / 2.0f), target_box_size);

                if (our_box.intersects(target_box))
                {
                    _collisions += zone->get_name() + String("\n");
                }
            }
        }
    }

    void ZoneNode3D::_ready()
    {
        CollisionShapeVisual3D::_ready();
        set_disabled(true);

        set_process(IS_EDITOR);

        EDITOR_ONLY();

        _timer.instantiate();
        _timer->set(3.0f);

        int32_t children = get_child_count();

        for (int32_t i = 0; i < children; ++i)
        {
            Label3D* label = cast_to<Label3D>(get_child(i));
            if (label && label->has_meta("ZoneLabel"))
            {
                label->queue_free();
            }
        }
        
        _label = memnew(Label3D);
        _label->set_billboard_mode(BaseMaterial3D::BillboardMode::BILLBOARD_ENABLED);
        _label->set_text(".");
        _label->set_font_size(48);
        _label->set_outline_size(6);
        _label->set_draw_flag(Label3D::DrawFlags::FLAG_DISABLE_DEPTH_TEST, true);
        _label->set_meta("ZoneLabel", true);
        add_child(_label, false, Node::INTERNAL_MODE_FRONT);

        _on_shape_data_changed();

        const Ref<Shape3D> shape = get_shape();

        if (shape == nullptr)
        {
            set_shape(Ref<BoxShape3D>(memnew(BoxShape3D)));
            BoxShape3D* box = cast_to<BoxShape3D>(get_shape().ptr());
            box->set_size(Vector3(250.0f, 50.f, 250.0f));
        }
    }

    void ZoneNode3D::_process(double p_delta)
    {
        EDITOR_ONLY();

        CollisionShapeVisual3D::_process(p_delta);

        Transform3D transform = get_transform();
        transform.basis = godot::base::Basis;

        if (transform.origin != _previous_position)
        {
            _on_shape_data_changed();
            _previous_position = transform.origin;
        }

        set_transform(transform);

        const Ref<Shape3D> shape = get_shape();

        if (shape != nullptr)
        {
            BoxShape3D* box_shape = cast_to<BoxShape3D>(shape.ptr());
            if (box_shape == nullptr)
            {
                set_shape(Ref<BoxShape3D>(memnew(BoxShape3D)));
                BoxShape3D* box = cast_to<BoxShape3D>(get_shape().ptr());
                box->set_size(Vector3(250.0f, 50.f, 250.0f));
            }
            else
            {
                if (_timer->update(p_delta))
                {
                    godot::String name = get_name();

                    if (name != _previous_name)
                    {
                        _label->set_text(name);
                        _previous_name = name;
                    }

                    Vector3 size = box_shape->get_size();
                    Vector3 pos(0.0f, size.y * 0.66f, 0.0f);

                    float font_size = 0.0f;

                    float x = Math::abs(size.x);
                    float z = Math::abs(size.z);

                    if (x > z)
                    {
                        font_size = x * 8.0f;
                    }
                    else
                    {
                        font_size = z * 8.0f;
                    }

                    if (font_size < 48.0f)
                    {
                        font_size = 48.0f;
                    }
                    else if (font_size > 3072.0f)
                    {
                        font_size = 3072.0f;
                    }

                    _label->set_position(pos);
                    _label->set_font_size(static_cast<int32_t>(font_size));
                    _label->set_outline_size(static_cast<int32_t>(font_size / 6.0f));
                }

            }
        }
    }

    void ZoneNode3D::show()
    {
        EDITOR_ONLY();

        _label->set_visible(true);
        get_visualizer_mesh()->set_visible(true);
    }

    void ZoneNode3D::hide()
    {
        EDITOR_ONLY();

        _label->set_visible(false);
        get_visualizer_mesh()->set_visible(false);
    }

}