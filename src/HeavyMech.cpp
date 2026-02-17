#include "HeavyMech.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


using namespace godot;

void HeavyMech::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("charge_attack", "direction"), &HeavyMech::charge_attack);
}

HeavyMech::HeavyMech()
{
  // Stats: Very High Health, High Damage, Low Movement
  set_max_health(200);
  set_current_health(200);
  set_attack_power(40);
  set_movement(2);  // Low mobility
  set_weight(40);   // Heavy weight for Ram
  set_name("Heavy Mech");
}

HeavyMech::~HeavyMech() {}

void HeavyMech::_enter_tree()
{
  if (get_texture().is_null()) {
    Ref<Texture2D> texture = ResourceLoader::get_singleton()->load("res://assets/mech_heavy.png");
    if (texture.is_valid()) {
      set_texture(texture);
    }
    else {
      UtilityFunctions::print("Error loading texture for HeavyMech");
    }
  }
}

void HeavyMech::charge_attack(Vector2 direction) { UtilityFunctions::print("HeavyMech charging!"); }

String HeavyMech::get_special_ability_description() const { return "Charge (Dash & Smash)"; }

void HeavyMech::use_special_ability()
{
  Vector2 mouse_pos = get_global_mouse_position();
  Vector2 direction = (mouse_pos - get_global_position()).normalized();
  charge_attack(direction);
}
