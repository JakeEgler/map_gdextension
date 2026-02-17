#include "ArtilleryMech.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


using namespace godot;

void ArtilleryMech::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("siege_mode"), &ArtilleryMech::siege_mode);
  ClassDB::bind_method(D_METHOD("get_is_siege_mode"), &ArtilleryMech::get_is_siege_mode);
}

ArtilleryMech::ArtilleryMech()
{
  // Stats: Low Health, High Damage
  set_max_health(80);
  set_current_health(80);
  set_attack_power(30);
  base_movement = 2;  // Low mobility
  set_movement(base_movement);
  set_name("Artillery Mech");
  is_siege_mode = false;
}

ArtilleryMech::~ArtilleryMech() {}

void ArtilleryMech::_enter_tree()
{
  if (get_texture().is_null()) {
    Ref<Texture2D> texture =
        ResourceLoader::get_singleton()->load("res://assets/mech_artillery.png");
    if (texture.is_valid()) {
      set_texture(texture);
    }
    else {
      UtilityFunctions::print("Error loading texture for ArtilleryMech");
    }
  }
}

void ArtilleryMech::siege_mode()
{
  is_siege_mode = !is_siege_mode;
  if (is_siege_mode) {
    set_movement(0);
    UtilityFunctions::print("ArtilleryMech entering Siege Mode! Movement is 0, Range increased.");
  }
  else {
    set_movement(base_movement);
    UtilityFunctions::print("ArtilleryMech leaving Siege Mode. Movement restored.");
  }
}

String ArtilleryMech::get_special_ability_description() const
{
  return is_siege_mode ? "Exit Siege Mode" : "Enter Siege Mode";
}

void ArtilleryMech::use_special_ability() { siege_mode(); }

void ArtilleryMech::use_second_ability()
{
  Unit* target = get_unit_under_mouse(get_global_mouse_position());
  if (target && target != this) {
    if (is_siege_mode) {
      UtilityFunctions::print("Firing Artillery in Siege Mode!");
    }
    ranged_attack(target);
  }
  else {
    UtilityFunctions::print("Shoot requires a target.");
  }
}

String ArtilleryMech::get_second_ability_description() const { return "Shoot"; }
