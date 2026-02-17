#ifndef SCOUT_MECH_H
#define SCOUT_MECH_H

#include "Player.h"

namespace godot {

class ScoutMech : public Player {
  GDCLASS(ScoutMech, Player);

protected:
  static void _bind_methods();

public:
  ScoutMech();
  ~ScoutMech();

  virtual void _enter_tree() override;

  void grapple(Unit *target);
  virtual String get_special_ability_description() const override;
  virtual void use_special_ability() override;

  // Override Ability 2 (default Ram) to be Ranged Attack
  virtual void use_second_ability() override;
  virtual String get_second_ability_description() const override;
};

} // namespace godot

#endif // SCOUT_MECH_H
