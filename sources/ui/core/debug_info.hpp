#pragma once

#include <ui/ui_component.hpp>

#include <godot_cpp/classes/e_net_multiplayer_peer.hpp>
#include <system/units/steam_multiplayer_peer.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>

#include <shared/units/sleep_block.hpp>
#include <shared/util.hpp>

namespace godot
{
    class RenderingServer;
    class Performance;
    class Label;
    class VBoxContainer;
}

namespace recusant
{
    class LaunchArgs;

    class DebugInfo : public UiComponent
    {
        GD_CLASS(DebugInfo, UiComponent);

    private:

        DEFINE_SINGLETON();

        enum ColoringType
        {
            HigherIsBetter = 0,
            LowerIsBetter,
        };

        struct GroupEntry
        {
            godot::String name;
            godot::String tip;
            ColoringType coloring;
        };

        struct ValueEntry
        {
            godot::String text;
            double value;
        };

        godot::Ref<SleepBlock> _timer;

        int _average_count = 8;

        LaunchArgs* _args = nullptr;
        godot::RID _viewport_rid;
        godot::RenderingServer* _server = nullptr;

        godot::Ref<godot::ENetMultiplayerPeer> _enet_peer;
        godot::Ref<SteamMultiplayerPeer> _steam_peer;
        godot::Ref<godot::MultiplayerAPI> _multiplayer;
        godot::Performance* _performance = nullptr;

        godot::Control* _root = nullptr;
        godot::VBoxContainer* _contents = nullptr;
        godot::VBoxContainer* _group = nullptr;
        godot::Label* _group_entry_name = nullptr;
        godot::Label* _group_entry_value = nullptr;

        void _add_group(godot::String p_name, const std::vector<GroupEntry>& p_values);

    protected:

        static void _bind_methods();

    public:

        IMPL_SINGLETON();

        IMPL_PROPERTY_NONCONST(godot::Control*, root);
        IMPL_PROPERTY_NONCONST(godot::VBoxContainer*, contents);
        IMPL_PROPERTY_NONCONST(godot::VBoxContainer*, group);
        IMPL_PROPERTY_NONCONST(godot::Label*, group_entry_name);
        IMPL_PROPERTY_NONCONST(godot::Label*, group_entry_value);

        void add_godot_metrics();
        void add_enet_metrics();
        void add_steam_metrics();

        void poll_godot_metrics();

        void poll();

        void initialize() override;
        void deinitialize() override;
        void _process(double p_delta) override;
    };
}
