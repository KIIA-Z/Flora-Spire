#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class PlayerAttackComponent : public Component {
protected:
  void attack(bool direction) const;
  void follow() const;
public:
  void update(double dt) override;
  void render() override {}
  explicit PlayerAttackComponent(Entity* p);
  PlayerAttackComponent() = delete;
};
