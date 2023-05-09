#pragma once

#include "engine.h"

class VictoryScene : public Scene {
public:
	VictoryScene() = default;
  ~VictoryScene() override = default;

  void Load() override;

  void Update(const double& dt) override;
};
