#include "TankMech.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void TankMech::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("gain_shield"), &TankMech::gain_shield);
}

TankMech::TankMech()
{
  // Stats: High Health, standard Movement
  set_max_health(150);
  set_current_health(150);
  set_attack_power(20);
  set_movement(3);  // Standard movement, can be adjusted
  set_name("Tank Mech");
  has_shield = false;
}

TankMech::~TankMech() {}

void TankMech::_enter_tree()
{
  if (get_texture().is_null()) {
    Ref<Texture2D> texture = ResourceLoader::get_singleton()->load("res://assets/mech_tank.png");
    if (texture.is_valid()) {
      set_texture(texture);
    }
    else {
      UtilityFunctions::print("Error loading texture for TankMech");
    }
  }
}

void TankMech::gain_shield()
{
  has_shield = true;
  UtilityFunctions::print("TankMech gained shield!");
}

void TankMech::take_damage(int p_amount)
{
  if (has_shield) {
    has_shield = false;
    UtilityFunctions::print("TankMech shield blocked damage!");
    // Shield absorbs the damage, so we don't call the parent implementation
  }
  else {
    Player::take_damage(p_amount);
  }
}

String TankMech::get_special_ability_description() const { return "Shield Up (Blocks next hit)"; }

void TankMech::use_special_ability() { gain_shield(); }
