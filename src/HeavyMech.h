#ifndef HEAVY_MECH_H
#define HEAVY_MECH_H

#include "Player.h"

namespace godot {

class HeavyMech : public Player {
  GDCLASS(HeavyMech, Player);

protected:
  static void _bind_methods();

public:
  HeavyMech();
  ~HeavyMech();

  virtual void _enter_tree() override;

  void charge_attack(Vector2 direction);
  virtual String get_special_ability_description() const override;
  virtual void use_special_ability() override;
};

} // namespace godot

#endif // HEAVY_MECH_H
