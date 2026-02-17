#include "Player.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Player::Player() {
  set_name("Player");
  set_max_health(30);
  set_current_health(30);
  set_attack_power(5);
  set_movement(5);
  set_weight(20); // Heavier than default unit (10)
  is_selected = false;

  // Enable input processing
  set_process_unhandled_input(true);
}

Player::~Player() {}

void Player::_ready() {
  // Calculate initial grid_position from spawn position
  // This handles units placed manually in the editor
  const int TILE_SIZE = 16;
  Vector2 pos = get_global_position();

  Array grid_pos;
  grid_pos.resize(2);
  grid_pos[0] = (int)(pos.x / TILE_SIZE);
  grid_pos[1] = (int)(pos.y / TILE_SIZE);
  set_grid_position(grid_pos);

  UtilityFunctions::print(get_name(), " initialized at grid (", grid_pos[0],
                          ", ", grid_pos[1], ") pixel: ", pos);
}

void Player::_process(double delta) {
  if (Engine::get_singleton()->is_editor_hint())
    return; // Don't animate in editor

  animation_timer += delta;

  // Frame-based sprite animation for idle
  // Cycle through frames based on animation_speed
  if (animation_timer >= animation_speed) {
    animation_timer = 0.0;
    current_frame_index++;

    // Loop back to frame 0 after reaching the last frame
    // hframes is set to 13 for the mech sprite sheets
    if (current_frame_index >= get_hframes()) {
      current_frame_index = 0;
    }

    set_frame(current_frame_index);
  }
}

void Player::_bind_methods() {
  ClassDB::bind_method(D_METHOD("move_to", "target_pos"), &Player::move_to);
  ClassDB::bind_method(D_METHOD("attack", "target"), &Player::attack);
  ClassDB::bind_method(D_METHOD("take_damage", "amount"), &Player::take_damage);
}

void Player::take_turn() {
  // Placeholder implementation for player turn logic
  // This will be overridden in mech subclasses with specific abilities
}

void Player::_unhandled_input(const Ref<InputEvent> &p_event) {
  Ref<InputEventMouseButton> mouse_event = p_event;
  if (mouse_event.is_valid() && mouse_event->is_pressed() &&
      mouse_event->get_button_index() == MOUSE_BUTTON_LEFT) {
    Vector2 mouse_pos = get_global_mouse_position();

    // Convert mouse pos to local space relative to parent or use global
    // position check.

    // If clicking on self -> Select
    Rect2 rect = get_rect();
    // Transform mouse to local space
    Vector2 local_mouse =
        get_global_transform().affine_inverse().xform(mouse_pos);

    if (rect.has_point(local_mouse)) {
      is_selected = !is_selected; // Toggle selection
      UtilityFunctions::print("Player Selected: ", is_selected);

      // Visual feedback
      if (is_selected) {
        set_modulate(Color(0.5, 0.5, 1.0)); // Blue tint
        print_action_options();
      } else {
        set_modulate(Color(1, 1, 1)); // Normal
      }

      get_viewport()->set_input_as_handled(); // Consume event so we don't move
                                              // immediately
      return;
    }

    // If selected and clicking elsewhere -> Move
    if (is_selected) {
      move_to(mouse_pos);  // Move to global mouse position
      is_selected = false; // Deselect after moving
      set_modulate(Color(1, 1, 1));
      get_viewport()->set_input_as_handled();
    }
  } else if (p_event.is_valid() && p_event->is_class("InputEventKey")) {
    if (!is_selected)
      return; // Ignore keys if not selected

    Ref<InputEventKey> key_event = p_event;
    if (key_event->is_pressed() && !key_event->is_echo()) {
      if (key_event->get_keycode() == KEY_1) {
        use_first_ability();
        get_viewport()->set_input_as_handled();
      } else if (key_event->get_keycode() == KEY_2) {
        use_second_ability();
        get_viewport()->set_input_as_handled();
      } else if (key_event->get_keycode() == KEY_3) {
        // Special Ability
        use_special_ability();
        get_viewport()->set_input_as_handled();
      }
    }
  }
}

void Player::move_to(const Vector2 &p_target_pos) {
  // Snap to tile center instead of exact mouse position
  const int TILE_SIZE = 16;

  // Convert mouse position to grid coordinates
  int grid_x = (int)(p_target_pos.x / TILE_SIZE);
  int grid_y = (int)(p_target_pos.y / TILE_SIZE);

  // Calculate center of the tile
  Vector2 tile_center;
  tile_center.x = (grid_x * TILE_SIZE) + (TILE_SIZE / 2.0f);
  tile_center.y = (grid_y * TILE_SIZE) + (TILE_SIZE / 2.0f);

  // Move to tile center
  set_global_position(tile_center);

  // Update grid_position
  Array grid_pos;
  grid_pos.resize(2);
  grid_pos[0] = grid_x;
  grid_pos[1] = grid_y;
  set_grid_position(grid_pos);

  UtilityFunctions::print("Player moved to tile (", grid_x, ", ", grid_y,
                          ") at position: ", tile_center);
}

void Player::attack(Unit *p_target) {
  if (!p_target)
    return;
  ram_attack(
      p_target); // Default attack is now basically ram? Or keep separate.
  // Keeping separate for now, but preserving original attack logic if needed.
  // Actually, let's just make attack() do a basic hit.

  UtilityFunctions::print(get_name(), " attacks ", p_target->get_name(),
                          " for ", get_attack_power(), " damage!");

  int new_health = p_target->get_current_health() - get_attack_power();
  if (new_health < 0)
    new_health = 0;
  p_target->set_current_health(new_health);

  UtilityFunctions::print(p_target->get_name(), " health is now ", new_health);
}

void Player::take_damage(int p_amount) {
  int new_health = get_current_health() - p_amount;
  if (new_health < 0)
    new_health = 0;
  set_current_health(new_health);

  UtilityFunctions::print(get_name(), " took ", p_amount,
                          " damage. Health: ", new_health);

  if (new_health <= 0) {
    UtilityFunctions::print(get_name(), " has died!");
    queue_free();
  }
}

void Player::heal() {
  int heal_amount = 5;
  int new_health = get_current_health() + heal_amount;
  if (new_health > get_max_health()) {
    new_health = get_max_health();
  }
  set_current_health(new_health);
  UtilityFunctions::print("Player healed for ", heal_amount,
                          ". Health: ", new_health);

  // Animation: Flash Green
  Ref<Tween> tween = create_tween();
  tween->tween_property(this, "modulate", Color(0, 1, 0), 0.2); // Green
  tween->tween_property(this, "modulate", Color(1, 1, 1),
                        0.2); // Back to normal
}

void Player::ram_attack(Unit *p_target) {
  if (!p_target)
    return;

  // Weight Formula
  int my_weight = get_weight();
  int target_weight = p_target->get_weight();
  if (target_weight <= 0)
    target_weight = 1; // Prevent div by zero
  if (my_weight <= 0)
    my_weight = 1;

  // Damage Dealt = Attack * (My / Target)
  float attack_mult = (float)my_weight / (float)target_weight;
  int damage_to_enemy = (int)(get_attack_power() * attack_mult);

  // Recoil = (Attack / 2) * (Target / My)
  float recoil_mult = (float)target_weight / (float)my_weight;
  int damage_to_self = (int)((get_attack_power() / 2.0f) * recoil_mult);

  // Ensure at least 1 damage if attack power > 0
  if (damage_to_enemy < 1 && get_attack_power() > 0)
    damage_to_enemy = 1;

  p_target->take_damage(damage_to_enemy);
  take_damage(damage_to_self);

  UtilityFunctions::print("Ram Attack! Weights (Me: ", my_weight,
                          " vs Target: ", target_weight, ")");
  UtilityFunctions::print("Dealt ", damage_to_enemy, " damage, took ",
                          damage_to_self, " recoil.");

  // Animation: Lunge
  Vector2 original_pos = get_position();
  Vector2 target_pos = p_target->get_position();
  Vector2 lunge_pos = original_pos.lerp(target_pos, 0.5); // Move halfway

  Ref<Tween> tween = create_tween();
  tween->tween_property(this, "position", lunge_pos, 0.1)
      ->set_trans(Tween::TRANS_QUAD)
      ->set_ease(Tween::EASE_OUT);
  tween->tween_property(this, "position", original_pos, 0.2)
      ->set_trans(Tween::TRANS_QUAD)
      ->set_ease(Tween::EASE_OUT);
}

void Player::ranged_attack(Unit *p_target) {
  if (!p_target)
    return;

  int damage = 8;
  p_target->take_damage(damage);

  UtilityFunctions::print("Ranged Blast! Dealt ", damage, " to ",
                          p_target->get_name());

  // Animation: Flash Target Red
  Ref<Tween> tween = create_tween();
  tween->tween_property(p_target, "modulate", Color(1, 0, 0), 0.1); // Red
  tween->tween_property(p_target, "modulate", Color(1, 1, 1), 0.1); // White
}

Unit *Player::get_unit_under_mouse(const Vector2 &p_global_mouse_pos) {
  // Use grid/tile-based detection instead of sprite bounds
  // This works regardless of sprite size or AnimatedSprite2D vs Sprite2D

  const int TILE_SIZE = 16; // pixels per tile (matches map.hpp)

  // Convert mouse position to grid coordinates
  int mouse_grid_x = (int)(p_global_mouse_pos.x / TILE_SIZE);
  int mouse_grid_y = (int)(p_global_mouse_pos.y / TILE_SIZE);

  UtilityFunctions::print("Mouse at pixel: ", p_global_mouse_pos, " -> Grid: (",
                          mouse_grid_x, ", ", mouse_grid_y, ")");

  // Get the scene tree root to search ALL units
  SceneTree *tree = get_tree();
  if (!tree)
    return nullptr;

  Window *root = tree->get_root();
  if (!root)
    return nullptr;

  // Get all nodes in the scene
  TypedArray<Node> all_nodes = root->find_children("*", "", true, false);

  int unit_count = 0;

  for (int i = 0; i < all_nodes.size(); i++) {
    Node *node = Object::cast_to<Node>(all_nodes[i]);
    Unit *unit = Object::cast_to<Unit>(node);
    if (unit) {
      unit_count++;

      // Get the unit's grid position
      Array grid_pos = unit->get_grid_position();
      if (grid_pos.size() >= 2) {
        int unit_grid_x = (int)grid_pos[0];
        int unit_grid_y = (int)grid_pos[1];

        bool is_on_tile =
            (unit_grid_x == mouse_grid_x && unit_grid_y == mouse_grid_y);

        UtilityFunctions::print("  Unit: ", unit->get_name(), " | Grid: (",
                                unit_grid_x, ", ", unit_grid_y, ")",
                                " | Match: ", is_on_tile);

        if (is_on_tile) {
          UtilityFunctions::print("  >>> Found unit on tile: ",
                                  unit->get_name());
          return unit;
        }
      }
    }
  }

  UtilityFunctions::print("  Total units checked: ", unit_count,
                          " | No unit on tile (", mouse_grid_x, ", ",
                          mouse_grid_y, ")");
  return nullptr;
}

void Player::print_action_options() {
  UtilityFunctions::print("========================================");
  UtilityFunctions::print("Unit: ", get_name());
  UtilityFunctions::print("HP: ", get_current_health(), "/", get_max_health(),
                          " | Move: ", get_movement());
  UtilityFunctions::print("Actions:");
  UtilityFunctions::print("1. Move (Click to move)");
  UtilityFunctions::print("1. ",
                          get_first_ability_description()); // Key 1 override
  UtilityFunctions::print("2. ", get_second_ability_description());
  UtilityFunctions::print("3. ", get_special_ability_description());
  UtilityFunctions::print("========================================");
}

String Player::get_special_ability_description() const { return "Wait"; }

void Player::use_special_ability() {
  UtilityFunctions::print("Player uses default ability: Wait.");
}

void Player::use_first_ability() { heal(); }

void Player::use_second_ability() {
  // Default is Ram Attack
  Unit *target = get_unit_under_mouse(get_global_mouse_position());
  if (target && target != this) {
    ram_attack(target);
  } else {
    UtilityFunctions::print("Ram Attack requires a target under mouse.");
  }
}

String Player::get_first_ability_description() const { return "Heal"; }

String Player::get_second_ability_description() const { return "Attack (Ram)"; }
