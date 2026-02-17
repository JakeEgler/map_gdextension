#ifndef TANK_MECH_H
#define TANK_MECH_H

#include "Player.h"

namespace godot {

class TankMech : public Player {
  GDCLASS(TankMech, Player);

private:
  bool has_shield;

protected:
  static void _bind_methods();

public:
  TankMech();
  ~TankMech();

  virtual void _enter_tree() override;

  void gain_shield();
  virtual void take_damage(int p_amount) override;
  virtual String get_special_ability_description() const override;
  virtual void use_special_ability() override;
};

} // namespace godot

#endif // TANK_MECH_H
