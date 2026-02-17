#include "ScoutMech.h"

#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void ScoutMech::_bind_methods() {
  ClassDB::bind_method(D_METHOD("grapple", "target"), &ScoutMech::grapple);
}

ScoutMech::ScoutMech() {
  // Stats: High Movement, Moderate Health
  set_max_health(100);
  set_current_health(100);
  set_attack_power(15);
  set_movement(5); // High mobility (default is around 3)
  set_name("Scout Mech");
}

ScoutMech::~ScoutMech() {}

void ScoutMech::_enter_tree() {
  if (get_texture().is_null()) {
    Ref<Texture2D> texture =
        ResourceLoader::get_singleton()->load("res://assets/mech_scout.png");
    if (texture.is_valid()) {
      set_texture(texture);
    } else {
      UtilityFunctions::print("Error loading texture for ScoutMech");
    }
  }
}

void ScoutMech::grapple(Unit *target) {
  if (!target) {
    UtilityFunctions::print("ScoutMech grapple failed: No target.");
    return;
  }

  UtilityFunctions::print("ScoutMech grappling " + target->get_name() + "!");

  // Pull target to the tile directly to the right of the scout
  const int TILE_SIZE = 16;

  // Get scout's grid position
  Array my_grid = get_grid_position();
  if (my_grid.size() < 2) {
    UtilityFunctions::print("ScoutMech has invalid grid position");
    return;
  }

  int my_grid_x = (int)my_grid[0];
  int my_grid_y = (int)my_grid[1];

  UtilityFunctions::print("ScoutMech is at grid (", my_grid_x, ", ", my_grid_y,
                          ") pixel position: ", get_global_position());

  // Calculate tile to the right (+1 on X axis)
  int target_grid_x = my_grid_x + 1;
  int target_grid_y = my_grid_y;

  // Calculate center of target tile
  Vector2 new_pos;
  new_pos.x = (target_grid_x * TILE_SIZE) + (TILE_SIZE / 2.0f);
  new_pos.y = (target_grid_y * TILE_SIZE) + (TILE_SIZE / 2.0f);

  UtilityFunctions::print("Pulling ", target->get_name(), " to tile (",
                          target_grid_x, ", ", target_grid_y, ")");

  // Animate target being pulled
  Ref<Tween> tween = create_tween();
  tween->tween_property(target, "position", new_pos, 0.3)
      ->set_trans(Tween::TRANS_QUAD)
      ->set_ease(Tween::EASE_OUT);

  // Update target's grid position
  Array target_grid_pos;
  target_grid_pos.resize(2);
  target_grid_pos[0] = target_grid_x;
  target_grid_pos[1] = target_grid_y;
  target->set_grid_position(target_grid_pos);
}

String ScoutMech::get_special_ability_description() const {
  return "Grapple (Pull Target)";
}

void ScoutMech::use_special_ability() {
  Unit *target = get_unit_under_mouse(get_global_mouse_position());
  if (target && target != this) {
    grapple(target);
  } else {
    UtilityFunctions::print("ScoutMech: No valid target for Grapple.");
  }
}

void ScoutMech::use_second_ability() {
  Unit *target = get_unit_under_mouse(get_global_mouse_position());
  if (target && target != this) {
    ranged_attack(target);
  } else {
    UtilityFunctions::print("Ranged Attack requires a target.");
  }
}

String ScoutMech::get_second_ability_description() const {
  return "Ranged Blast";
}
