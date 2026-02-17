#ifndef UG_HF_6_BACKEND_SRC_PLAYER_H
#define UG_HF_6_BACKEND_SRC_PLAYER_H

#include "Unit.h"
#include <godot_cpp/classes/input_event.hpp>

namespace godot {

class Player : public Unit {
  GDCLASS(Player, Unit);

private:
  bool is_selected;

protected:
  static void _bind_methods();

public:
  Player();
  ~Player();

  void _ready() override;
  void _unhandled_input(const Ref<InputEvent> &p_event) override;
  void _process(double delta) override;

  virtual void take_turn() override;

  void move_to(const Vector2 &p_target_pos);
  void attack(Unit *p_target);
  void take_damage(int p_amount);

  // New Abilities
  void heal();
  void ram_attack(Unit *p_target);
  void ranged_attack(Unit *p_target);

  // Virtual Ability Slots (Key 1, Key 2)
  virtual void use_first_ability();
  virtual void use_second_ability();
  virtual String get_first_ability_description() const;
  virtual String get_second_ability_description() const;

  // Helper
  Unit *get_unit_under_mouse(const Vector2 &p_global_mouse_pos);

  virtual void print_action_options();
  virtual String get_special_ability_description() const;
  virtual void use_special_ability();
};

} // namespace godot

#endif // UG_HF_6_BACKEND_SRC_PLAYER_H
