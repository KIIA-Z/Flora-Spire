#pragma once

#include "engine.h"

class SettingsScene : public Scene {
private:
	std::vector<sf::Vector2f> posMenuItems;
public:
	SettingsScene() = default;
  ~SettingsScene() override = default;

  void Load() override;

  void Update(const double& dt) override;
};