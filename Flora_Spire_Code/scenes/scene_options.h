#pragma once

#include "engine.h"

class OptionsScene : public Scene {
private:
	std::vector<sf::Vector2f> posMenuItems;
public:
	OptionsScene() = default;
  ~OptionsScene() override = default;

  void Load() override;

  void Update(const double& dt) override;
};