#include <register_types.hpp>

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

// Resources
#include <resources/item.hpp>
// Shared units
#include <shared/group.hpp>
#include <shared/units/sleep_block.hpp>
#include <shared/units/motion_generator.hpp>
// Shared code
#include <shared/entity.hpp>
#include <shared/synchronizers/base_synchronizer.hpp>
#include <shared/synchronizers/authority_synchronizer.hpp>
#include <shared/synchronizers/entity_synchronizer.hpp>
#include <shared/entity_spawner.hpp>
#include <shared/components/component.hpp>
#include <shared/components/snapshot.hpp>
#include <shared/components/interactable.hpp>
// Level scenes
#include <level/collision_shape_visual_manager.hpp>
#include <level/collision_shape_visual_3d.hpp>
#include <level/level_zoning.hpp>
#include <level/zone_node_3d.hpp>
#include <level/door/door_interactable.hpp>
#include <level/door/pathfinding.hpp>
#include <level/trigger_multiple.hpp>
#include <level/drill/drill_resources.hpp>
#include <level/drill/drill_interactable.hpp>
// Dynamic scenes
#include <dynamic/player/player_movement.hpp>
#include <dynamic/player/player_snapshot.hpp>
#include <dynamic/player/label.hpp>
#include <dynamic/player/callout.hpp>
#include <dynamic/player/player_interactor.hpp>
#include <dynamic/player/shooter.hpp>
#include <dynamic/soldier/soldier_snapshot.hpp>
#include <dynamic/soldier/pathfinding.hpp>
// System units
#include <system/units/steam_multiplayer_peer.hpp>
#include <system/units/steam_callback.hpp>
// System scenes
#include <system/core.hpp>
#include <system/logger.hpp>
#include <system/launch_args.hpp>
#include <system/steam.hpp>
#include <system/network.hpp>
#include <system/player/spawner.hpp>
#include <system/commander.hpp>
#include <system/level_manager.hpp>
#include <system/ui_root.hpp>
// UI scenes
#include <ui/core/core_state.hpp>
#include <ui/core/debug_info.hpp>
#include <ui/escape/escape_state.hpp>
#include <ui/gameplay/gameplay_state.hpp>
#include <ui/gameplay/chat.hpp>
#include <ui/gameplay/interact_menu.hpp>
#include <ui/loading/loading_state.hpp>
#include <ui/settings/settings_state.hpp>

#define BIND_CLASS(m_class) \
    do { \
    ::godot::ClassDB::register_class<recusant::m_class>(); \
    } while (0)

#define BIND_CLASS_VIRTUAL(m_class) \
    do { \
    ::godot::ClassDB::register_class<recusant::m_class>(true); \
    } while (0)

#define BIND_CLASS_ABSTRACT(m_class) \
    do { \
    ::godot::ClassDB::register_abstract_class<recusant::m_class>(); \
    } while (0)

void initialize_recusant(godot::ModuleInitializationLevel p_level)
{
    if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        return;
    }
    // Resources
    BIND_CLASS(Item);
    // Shared units
    BIND_CLASS(Group);
    BIND_CLASS(SleepBlock);
    BIND_CLASS(MotionGenerator);
    // Shared code
    BIND_CLASS(Entity);
    BIND_CLASS(BaseSynchronizer);
    BIND_CLASS(AuthoritySynchronizer);
    BIND_CLASS(EntitySynchronizer);
    BIND_CLASS(EntitySpawner);
    BIND_CLASS(Component);
    BIND_CLASS_VIRTUAL(Snapshot);
    BIND_CLASS(Interactable);
    // Level scenes
    BIND_CLASS(CollisionShapeVisualManager);
    BIND_CLASS(CollisionShapeVisual3D);
    BIND_CLASS(ZoneNode3D);
    BIND_CLASS(LevelZoning);
    BIND_CLASS(DoorInteractable);
    BIND_CLASS(DoorPathfinding);
    BIND_CLASS(TriggerMultiple);
    BIND_CLASS(DrillResources);
    BIND_CLASS(DrillInteractable);
    // Dynamic scenes
    BIND_CLASS(PlayerMovement);
    BIND_CLASS(PlayerSnapshot);
    BIND_CLASS(PlayerLabel);
    BIND_CLASS(PlayerCallout);
    BIND_CLASS(PlayerInteractor);
    BIND_CLASS(PlayerShooter);
    BIND_CLASS(SoldierSnapshot);
    BIND_CLASS(SoldierPathfinding);
    // System units
    BIND_CLASS(SteamMultiplayerPeer);
    BIND_CLASS(SteamCallback);
    // System scenes
    BIND_CLASS(System);
    BIND_CLASS(Logger);
    BIND_CLASS(Core);
    BIND_CLASS(LaunchArgs);
    BIND_CLASS(Steam);
    BIND_CLASS(Network);
    BIND_CLASS(PlayerSpawner);
    BIND_CLASS(Commander);
    BIND_CLASS(LevelManager);
    // UI scenes
    BIND_CLASS(UiRoot);
    BIND_CLASS(UiState);
    BIND_CLASS(UiComponent);
    BIND_CLASS(CoreState);
    BIND_CLASS(DebugInfo);
    BIND_CLASS(EscapeState);
    BIND_CLASS(GameplayState);
    BIND_CLASS(Chat);
    BIND_CLASS(InteractMenu);
    BIND_CLASS(LoadingState);
    BIND_CLASS(SettingsState);
}

void uninitialize_recusant(godot::ModuleInitializationLevel p_level)
{
    if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        return;
    }
}

extern "C"
{
    GDExtensionBool GDE_EXPORT recusant_game_init(
        GDExtensionInterfaceGetProcAddress p_get_proc_address,
        GDExtensionClassLibraryPtr p_library,
        GDExtensionInitialization* r_initialization)
    {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_recusant);
        init_obj.register_terminator(uninitialize_recusant);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}
