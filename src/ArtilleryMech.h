#ifndef ARTILLERY_MECH_H
#define ARTILLERY_MECH_H

#include "Player.h"

namespace godot {

class ArtilleryMech : public Player {
  GDCLASS(ArtilleryMech, Player);

private:
  bool is_siege_mode;
  int base_movement;

protected:
  static void _bind_methods();

public:
  ArtilleryMech();
  ~ArtilleryMech();

  virtual void _enter_tree() override;

  void siege_mode();
  bool get_is_siege_mode() const { return is_siege_mode; }
  virtual String get_special_ability_description() const override;
  virtual void use_special_ability() override;

  // Override Ability 2 (default Ram) to be Shoot
  virtual void use_second_ability() override;
  virtual String get_second_ability_description() const override;
};

} // namespace godot

#endif // ARTILLERY_MECH_H
