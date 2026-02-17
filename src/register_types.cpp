#include "register_types.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "ArtilleryMech.h"
#include "Enemy.h"
#include "GameManager.h"
#include "HeavyMech.h"
#include "Player.h"
#include "ScoutMech.h"
#include "TankMech.h"
#include "Unit.h"
#include "map.hpp"
#include "tile.hpp"

using namespace godot;

void initialize_game_extension_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_ABSTRACT_CLASS(Unit);
	GDREGISTER_CLASS(Enemy);
	GDREGISTER_CLASS(Player);
	GDREGISTER_CLASS(TankMech);
	GDREGISTER_CLASS(ArtilleryMech);
	GDREGISTER_CLASS(ScoutMech);
	GDREGISTER_CLASS(HeavyMech);

	// Register Map and Tile classes
	GDREGISTER_CLASS(Tile);
	GDREGISTER_CLASS(Map);
	GDREGISTER_CLASS(GameManager);
}

void uninitialize_game_extension_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT
plugin_name_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_game_extension_module);
	init_obj.register_terminator(uninitialize_game_extension_module);
	init_obj.set_minimum_library_initialization_level(
			MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
