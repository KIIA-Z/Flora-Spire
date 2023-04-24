#pragma once

#include "engine.h"

class MenuScene : public Scene {
private:
	std::vector<sf::Vector2f> posMenuItems;
public:
  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void Update(const double& dt) override;
  void Render() override;

};
