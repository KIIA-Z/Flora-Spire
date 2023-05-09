#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class MeleeComponent : public Component {
protected:
  float _lifetime;

public:
  void update(double dt) override;
  void render() override {}
  explicit MeleeComponent(Entity* p, float lifetime = 0.5f);
  MeleeComponent() = delete;
};
