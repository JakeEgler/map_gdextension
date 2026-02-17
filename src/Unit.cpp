#include "Unit.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

Unit::Unit() {
  movement = 3; // Default movement
  max_health = 10;
  current_health = 10;
  attack_power = 2;
  weight = 10;
  has_acted = false;
  unit_name = "Unit";
  grid_position.resize(2);
  grid_position[0] = 0;
  grid_position[1] = 0;
  pixel_position = Vector2(0, 0);

  // Animation Defaults
  animation_timer = 0.0;
  animation_speed = 0.15; // Speed of idle animation
  current_frame_index = 0;

  set_hframes(13); // Default to 13 frames for the mechs sprite sheets
  set_frame(0);    // Start at the first frame
}
void Unit::set_pixel_position(const Vector2 &p_position) {
  pixel_position = p_position;
}

Vector2 Unit::get_pixel_position() const {
  return pixel_position;
}

Unit::~Unit() {}

void Unit::move_to_position(const Vector2& p_position) {
    set_global_position(p_position);
    pixel_position = p_position;
}

void Unit::_process(double delta) {
  if (Engine::get_singleton()->is_editor_hint())
    return; // Don't animate in editor if unwanted

  animation_timer += delta;

  // Bobbing Animation (Up and Down)
  // Speed: 5.0, Amplitude: 2.0 pixels
  float bob_offset = Math::sin(animation_timer * 5.0) * 2.0;

  // Ensure we display the first frame (since we have hframes=3 but want static
  // image bobbing) If the user wants the "3 robots" image to be a single frame,
  // hframes should be 1. BUT the user said "back to being 3 robots per unit"
  // when hframes was 1. If the user wants to see ONE robot, they need hframes
  // to match the sheet count. User said "set hframes to 1 as 3 mechs are still
  // displayed" -> then reported "back to being 3 robots". This implies
  // hframes=1 SHOWS 3 robots. To show 1 robot, hframes must be 3 (if there are
  // 3 on the sheet).

  set_offset(Vector2(0, bob_offset));
}

void Unit::set_movement(int p_movement) { movement = p_movement; }

int Unit::get_movement() const { return movement; }

void Unit::set_grid_position(const Array &p_position) {
  if (p_position.size() >= 2) {
    grid_position = p_position.duplicate();
  }
}

Array Unit::get_grid_position() const { return grid_position; }

void Unit::set_max_health(int p_health) { max_health = p_health; }

int Unit::get_max_health() const { return max_health; }

void Unit::set_current_health(int p_health) { current_health = p_health; }

int Unit::get_current_health() const { return current_health; }

void Unit::set_attack_power(int p_power) { attack_power = p_power; }

int Unit::get_attack_power() const { return attack_power; }

void Unit::set_weight(int p_weight) { weight = p_weight; }

int Unit::get_weight() const { return weight; }

void Unit::set_has_acted(bool p_acted) { has_acted = p_acted; }

bool Unit::get_has_acted() const { return has_acted; }

void Unit::set_name(const String &p_name) { unit_name = p_name; }

String Unit::get_name() const { return unit_name; }

void Unit::take_damage(int p_amount) {
  current_health -= p_amount;
  if (current_health < 0)
    current_health = 0;
}

void Unit::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_pixel_position", "pixel_position"), &Unit::set_pixel_position);
    ClassDB::bind_method(D_METHOD("get_pixel_position"), &Unit::get_pixel_position);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "pixel_position"), "set_pixel_position", "get_pixel_position");
  ClassDB::bind_method(D_METHOD("set_movement", "movement"),
                       &Unit::set_movement);
  ClassDB::bind_method(D_METHOD("get_movement"), &Unit::get_movement);
  ADD_PROPERTY(PropertyInfo(Variant::INT, "movement"), "set_movement",
               "get_movement");

  ClassDB::bind_method(D_METHOD("set_grid_position", "position"),
                       &Unit::set_grid_position);
  ClassDB::bind_method(D_METHOD("get_grid_position"), &Unit::get_grid_position);
  ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "grid_position"),
               "set_grid_position", "get_grid_position");

  ClassDB::bind_method(D_METHOD("set_max_health", "health"),
                       &Unit::set_max_health);
  ClassDB::bind_method(D_METHOD("get_max_health"), &Unit::get_max_health);
  ADD_PROPERTY(PropertyInfo(Variant::INT, "max_health"), "set_max_health",
               "get_max_health");

  ClassDB::bind_method(D_METHOD("set_current_health", "health"),
                       &Unit::set_current_health);
  ClassDB::bind_method(D_METHOD("get_current_health"),
                       &Unit::get_current_health);
  ADD_PROPERTY(PropertyInfo(Variant::INT, "current_health"),
               "set_current_health", "get_current_health");

  ClassDB::bind_method(D_METHOD("set_attack_power", "power"),
                       &Unit::set_attack_power);
  ClassDB::bind_method(D_METHOD("get_attack_power"), &Unit::get_attack_power);
  ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_power"), "set_attack_power",
               "get_attack_power");

  ClassDB::bind_method(D_METHOD("set_weight", "weight"), &Unit::set_weight);
  ClassDB::bind_method(D_METHOD("get_weight"), &Unit::get_weight);
  ADD_PROPERTY(PropertyInfo(Variant::INT, "weight"), "set_weight",
               "get_weight");

  ClassDB::bind_method(D_METHOD("set_has_acted", "acted"),
                       &Unit::set_has_acted);
  ClassDB::bind_method(D_METHOD("get_has_acted"), &Unit::get_has_acted);
  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_acted"), "set_has_acted",
               "get_has_acted");

  ClassDB::bind_method(D_METHOD("set_name", "name"), &Unit::set_name);
  ClassDB::bind_method(D_METHOD("get_name"), &Unit::get_name);
  ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");

  ClassDB::bind_method(D_METHOD("take_turn"), &Unit::take_turn);
}
