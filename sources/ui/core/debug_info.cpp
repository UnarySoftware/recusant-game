#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/input.hpp>

#include <godot_cpp/classes/performance.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/viewport.hpp>


#include <system/launch_args.hpp>
#include <system/network.hpp>

#include <vector>

#include <ui/core/debug_info.hpp>

using namespace godot;

namespace recusant
{
    DECLARE_SINGLETON(DebugInfo);

    void DebugInfo::_bind_methods()
    {
        BIND_SINGLETON();

        BIND_PROPERTY_PATH(Control, root);
        BIND_PROPERTY_PATH(VBoxContainer, contents);
        BIND_PROPERTY_PATH(VBoxContainer, group);
        BIND_PROPERTY_PATH(Label, group_entry_name);
        BIND_PROPERTY_PATH(Label, group_entry_value);
    }

    void DebugInfo::_add_group(String p_name, const std::vector<GroupEntry>& p_values)
    {
        VBoxContainer* new_group = cast_to<VBoxContainer>(_group->duplicate());
        new_group->set_visible(true);
        new_group->get_node<Label>("name")->set_text(p_name);
        new_group->set_name(p_name);

        VBoxContainer* names = new_group->get_node<VBoxContainer>("variables/names");
        VBoxContainer* values = new_group->get_node<VBoxContainer>("variables/values");

        for (const auto& variable : p_values)
        {
            String tooltip = variable.tip;
            if (variable.coloring == ColoringType::HigherIsBetter)
            {
                tooltip += "\nHigher is better.";
            }
            else
            {
                tooltip += "\nLower is better.";
            }
            Label* new_name = cast_to<Label>(_group_entry_name->duplicate());
            new_name->set_visible(true);
            new_name->set_name(variable.name);
            new_name->set_text(String(variable.name) + ":");
            new_name->set_meta("coloring", variable.coloring);
            new_name->set_tooltip_text(tooltip);
            names->add_child(new_name);
            Label* new_value = cast_to<Label>(_group_entry_value->duplicate());
            new_value->set_visible(true);
            new_value->set_name(variable.name);
            new_value->set_text("0");
            new_value->set_tooltip_text(tooltip);
            values->add_child(new_value);
        }

        _contents->add_child(new_group);
    }

    void DebugInfo::add_godot_metrics()
    {
        std::vector<GroupEntry> values;
        GroupEntry entry;

        entry.name = "FPS";
        entry.coloring = ColoringType::HigherIsBetter;
        entry.tip = "The number of frames rendered in the last second.";
        values.push_back(entry);

        entry.name = "Process Time";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Time it took to complete one frame, in milliseconds.";
        values.push_back(entry);

        entry.name = "Physics Time";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Time it took to complete one physics frame, in milliseconds.";
        values.push_back(entry);

        entry.name = "Navigation Time";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Time it took to complete one navigation step, in milliseconds.\n") +
            "This includes navigation map updates as well as agent avoidance calculations.";
        values.push_back(entry);

#ifdef DEBUG_ENABLED
        entry.name = "Memory static";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Static memory currently used, in bytes.\n") +
            "Not available in release builds.";
        values.push_back(entry);

        entry.name = "Memory static max";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Available static memory.\n") +
            "Not available in release builds.";
        values.push_back(entry);
#endif

        entry.name = "Message buffer max";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Largest amount of memory the message queue buffer has used, in bytes.\n") +
            "The message queue is used for deferred functions calls and notifications.";
        values.push_back(entry);

        entry.name = "Object count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of objects currently instantiated (including nodes).";
        values.push_back(entry);

        entry.name = "Resources count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of resources currently used.";
        values.push_back(entry);

        entry.name = "Node count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Number of nodes currently instantiated in the scene tree.\n") +
            "This also includes the root node.";
        values.push_back(entry);

        entry.name = "Orphan node count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of orphan nodes, i.e. nodes which are not parented to a node of the scene tree.";
        values.push_back(entry);

        entry.name = "Total objects 3D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("The total number of objects in the last rendered frame for 3D.\n") +
            "This metric doesn't include culled objects (either via hiding nodes, frustum culling or occlusion culling).";
        values.push_back(entry);

        entry.name = "Total vertices 3D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("The total number of vertices or indices rendered in the last rendered frame for 3D.\n") +
            "This metric doesn't include primitives from culled objects (either via hiding nodes, frustum culling or occlusion culling).\n" +
            "Due to the depth prepass and shadow passes, the number of primitives is always higher than the actual number of vertices in the scene (typically double or triple the original vertex count).";
        values.push_back(entry);

        entry.name = "Total draw calls 3D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("The total number of draw calls performed in the last rendered frame for 3D.\n") +
            "This metric doesn't include culled objects (either via hiding nodes, frustum culling or occlusion culling), since they do not result in draw calls.";
        values.push_back(entry);
        
        entry.name = "Total objects 2D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("The total number of objects in the last rendered frame for 2D.\n") +
            "This metric doesn't include culled objects (either via hiding nodes, frustum culling or occlusion culling).";
        values.push_back(entry);

        entry.name = "Total vertices 2D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("The total number of vertices or indices rendered in the last rendered frame for 2D.\n") +
            "This metric doesn't include primitives from culled objects (either via hiding nodes, frustum culling or occlusion culling).\n" +
            "Due to the depth prepass and shadow passes, the number of primitives is always higher than the actual number of vertices in the scene (typically double or triple the original vertex count).";
        values.push_back(entry);

        entry.name = "Total draw calls 2D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("The total number of draw calls performed in the last rendered frame for 2D.\n") +
            "This metric doesn't include culled objects (either via hiding nodes, frustum culling or occlusion culling), since they do not result in draw calls.";
        values.push_back(entry);

        entry.name = "Video memory usage";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("The amount of video memory used (texture and vertex memory combined, in bytes).\n") +
            "Since this metric also includes miscellaneous allocations, this value is always greater than the sum of \"Texture memory\" and \"Buffer memory\".";
        values.push_back(entry);

        entry.name = "Texture memory usage";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "The amount of texture memory used (in bytes).";
        values.push_back(entry);

        entry.name = "Buffer memory usage";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "The amount of render buffer memory used (in bytes).";
        values.push_back(entry);

        entry.name = "Active objects 2D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of active RigidBody2D nodes in the game.";
        values.push_back(entry);

        entry.name = "Collision pairs 2D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of collision pairs in the 2D physics engine.";
        values.push_back(entry);

        entry.name = "Island count 2D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of islands in the 2D physics engine.";
        values.push_back(entry);

        entry.name = "Active objects 3D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of active RigidBody3D and VehicleBody3D nodes in the game.";
        values.push_back(entry);

        entry.name = "Collision pairs 3D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of collision pairs in the 3D physics engine.";
        values.push_back(entry);

        entry.name = "Island count 3D";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of islands in the 3D physics engine.";
        values.push_back(entry);

        entry.name = "Audio latency";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Output latency of the AudioServer.";
        values.push_back(entry);

        entry.name = "Active maps count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Number of active navigation maps in the NavigationServer3D.\n") +
            "This also includes the two empty default navigation maps created by World2D and World3D.";
        values.push_back(entry);

        entry.name = "Region count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of active navigation regions in the NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Agent count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of active navigation agents processing avoidance in the NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Link count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of active navigation links in the NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Polygon count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of navigation mesh polygons in the NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Edge count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of navigation mesh polygon edges in the NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Edge merge count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of navigation mesh polygon edges that were merged due to edge key overlap in the NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Edge connect count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = "Number of polygon edges that are considered connected by edge proximity NavigationServer3D.";
        values.push_back(entry);

        entry.name = "Edge free count";
        entry.coloring = ColoringType::LowerIsBetter;
        entry.tip = String("Number of navigation mesh polygon edges that could not be merged in the NavigationServer3D.\n") +
            "The edges still may be connected by edge proximity or with links.";
        values.push_back(entry);

        _add_group("Engine metrics", values);
    }

    void DebugInfo::add_enet_metrics()
    {
    }

    void DebugInfo::add_steam_metrics()
    {
    }

    void DebugInfo::poll_godot_metrics()
    {
        godot::HashMap<String, ValueEntry> values;
        ValueEntry entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::TIME_FPS));
        entry.text = String::num_uint64(entry.value);
        values["FPS"] = entry;

        entry.value = _performance->get_monitor(Performance::Monitor::TIME_PROCESS);
        entry.text = String::num_scientific(entry.value * 1'000) + " ms";
        values["Process Time"] = entry;

        entry.value = _performance->get_monitor(Performance::Monitor::TIME_PHYSICS_PROCESS);
        entry.text = String::num_scientific(entry.value * 1'000) + " ms";
        values["Physics Time"] = entry;

        entry.value = _performance->get_monitor(Performance::Monitor::TIME_NAVIGATION_PROCESS);
        entry.text = String::num_scientific(entry.value * 1'000) + " ms";
        values["Navigation Time"] = entry;

#ifdef DEBUG_ENABLED
        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::MEMORY_STATIC));
        entry.text = String::humanize_size(entry.value);
        values["Memory static"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::MEMORY_STATIC_MAX));
        entry.text = String::humanize_size(entry.value);
        values["Memory static max"] = entry;
#endif

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::MEMORY_MESSAGE_BUFFER_MAX));
        entry.text = String::humanize_size(entry.value);
        values["Message buffer max"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::OBJECT_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Object count"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::OBJECT_RESOURCE_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Resources count"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::OBJECT_NODE_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Node count"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::OBJECT_ORPHAN_NODE_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Orphan node count"] = entry;

        // 3D render info.

        int32_t objects_drawn_3d = _server->viewport_get_render_info(_viewport_rid,
            RenderingServer::ViewportRenderInfoType::VIEWPORT_RENDER_INFO_TYPE_VISIBLE,
            RenderingServer::ViewportRenderInfo::VIEWPORT_RENDER_INFO_OBJECTS_IN_FRAME) +
        _server->viewport_get_render_info(_viewport_rid,
            RenderingServer::ViewportRenderInfoType::VIEWPORT_RENDER_INFO_TYPE_SHADOW,
            RenderingServer::ViewportRenderInfo::VIEWPORT_RENDER_INFO_OBJECTS_IN_FRAME);

        int32_t vertices_drawn_3d = _server->viewport_get_render_info(_viewport_rid,
            RenderingServer::ViewportRenderInfoType::VIEWPORT_RENDER_INFO_TYPE_VISIBLE,
            RenderingServer::ViewportRenderInfo::VIEWPORT_RENDER_INFO_PRIMITIVES_IN_FRAME) +
        _server->viewport_get_render_info(_viewport_rid,
            RenderingServer::ViewportRenderInfoType::VIEWPORT_RENDER_INFO_TYPE_SHADOW,
            RenderingServer::ViewportRenderInfo::VIEWPORT_RENDER_INFO_PRIMITIVES_IN_FRAME);

        int32_t draw_calls_used_3d = _server->viewport_get_render_info(_viewport_rid,
            RenderingServer::ViewportRenderInfoType::VIEWPORT_RENDER_INFO_TYPE_VISIBLE,
            RenderingServer::ViewportRenderInfo::VIEWPORT_RENDER_INFO_DRAW_CALLS_IN_FRAME) +
        _server->viewport_get_render_info(_viewport_rid,
            RenderingServer::ViewportRenderInfoType::VIEWPORT_RENDER_INFO_TYPE_SHADOW,
            RenderingServer::ViewportRenderInfo::VIEWPORT_RENDER_INFO_DRAW_CALLS_IN_FRAME);

        entry.value = objects_drawn_3d;
        entry.text = String::num_uint64(entry.value);
        values["Total objects 3D"] = entry;

        entry.value = vertices_drawn_3d;
        entry.text = String::num_uint64(entry.value);
        values["Total vertices 3D"] = entry;

        entry.value = draw_calls_used_3d;
        entry.text = String::num_uint64(entry.value);
        values["Total draw calls 3D"] = entry;

        // 2D render info.
        
        int32_t objects_drawn_2d = _server->viewport_get_render_info(_viewport_rid,
            RenderingServer::ViewportRenderInfoType::VIEWPORT_RENDER_INFO_TYPE_CANVAS,
            RenderingServer::ViewportRenderInfo::VIEWPORT_RENDER_INFO_OBJECTS_IN_FRAME);

        int32_t vertices_drawn_2d = _server->viewport_get_render_info(_viewport_rid,
            RenderingServer::ViewportRenderInfoType::VIEWPORT_RENDER_INFO_TYPE_CANVAS,
            RenderingServer::ViewportRenderInfo::VIEWPORT_RENDER_INFO_PRIMITIVES_IN_FRAME);

        int32_t draw_calls_used_2d = _server->viewport_get_render_info(_viewport_rid,
            RenderingServer::ViewportRenderInfoType::VIEWPORT_RENDER_INFO_TYPE_CANVAS,
            RenderingServer::ViewportRenderInfo::VIEWPORT_RENDER_INFO_DRAW_CALLS_IN_FRAME);

        entry.value = objects_drawn_2d;
        entry.text = String::num_uint64(entry.value);
        values["Total objects 2D"] = entry;

        entry.value = vertices_drawn_2d;
        entry.text = String::num_uint64(entry.value);
        values["Total vertices 2D"] = entry;

        entry.value = draw_calls_used_2d;
        entry.text = String::num_uint64(entry.value);
        values["Total draw calls 2D"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::RENDER_VIDEO_MEM_USED));
        entry.text = String::humanize_size(entry.value);
        values["Video memory usage"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::RENDER_TEXTURE_MEM_USED));
        entry.text = String::humanize_size(entry.value);
        values["Texture memory usage"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::RENDER_BUFFER_MEM_USED));
        entry.text = String::humanize_size(entry.value);
        values["Buffer memory usage"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::PHYSICS_2D_ACTIVE_OBJECTS));
        entry.text = String::num_uint64(entry.value);
        values["Active objects 2D"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::PHYSICS_2D_COLLISION_PAIRS));
        entry.text = String::num_uint64(entry.value);
        values["Collision pairs 2D"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::PHYSICS_2D_ISLAND_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Island count 2D"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::PHYSICS_3D_ACTIVE_OBJECTS));
        entry.text = String::num_uint64(entry.value);
        values["Active objects 3D"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::PHYSICS_3D_COLLISION_PAIRS));
        entry.text = String::num_uint64(entry.value);
        values["Collision pairs 3D"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::PHYSICS_3D_ISLAND_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Island count 3D"] = entry;

        entry.value = _performance->get_monitor(Performance::Monitor::AUDIO_OUTPUT_LATENCY);
        entry.text = String::num_scientific(entry.value * 1'000) + " ms";
        values["Audio latency"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::NAVIGATION_ACTIVE_MAPS));
        entry.text = String::num_uint64(entry.value);
        values["Active maps count"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::NAVIGATION_REGION_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Region count"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::NAVIGATION_AGENT_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Agent count"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::NAVIGATION_LINK_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Link count"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::NAVIGATION_POLYGON_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Polygon count"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::NAVIGATION_EDGE_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Edge count"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::NAVIGATION_EDGE_MERGE_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Edge merge count"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::NAVIGATION_EDGE_CONNECTION_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Edge connect count"] = entry;

        entry.value = static_cast<uint32_t>(_performance->get_monitor(Performance::Monitor::NAVIGATION_EDGE_FREE_COUNT));
        entry.text = String::num_uint64(entry.value);
        values["Edge free count"] = entry;

        VBoxContainer* godot_group = _contents->get_node<VBoxContainer>("Engine metrics");
        for (const auto& target : values)
        {
            godot_group->get_node<Label>("variables/values/" + target.key)->set_text(target.value.text);
        }
    }

    void DebugInfo::poll()
    {
        poll_godot_metrics();
    }

    void DebugInfo::initialize()
    {
        ASSIGN_SINGLETON();
        
        _timer.instantiate();
        _timer->set(1.0f);

        _args = LaunchArgs::get_singleton();
        _viewport_rid = get_viewport()->get_viewport_rid();
        _server = RenderingServer::get_singleton();


        if (_args->is_steam())
        {
            _steam_peer = Network::get_singleton()->get_steam_peer();
        }
        else
        {
            _enet_peer = Network::get_singleton()->get_enet_peer();
        }

        _multiplayer = get_tree()->get_multiplayer();
        _performance = Performance::get_singleton();

        _group_entry_name->set_visible(false);
        _group_entry_value->set_visible(false);
        _group->set_visible(false);

        add_godot_metrics();

        _root->hide();
    }

    void DebugInfo::deinitialize()
    {
        UNASSIGN_SINGLETON();
    }

    void DebugInfo::_process(double p_delta)
    {
        RUNTIME_ONLY();

        if (Input::get_singleton()->is_action_just_released("ui_debug_info"))
        {
            if (_root->is_visible())
            {
                _root->hide();
            }
            else
            {
                _root->show();
            }
        }

        if (_timer->update(p_delta))
        {
            poll();
        }
    }
}
