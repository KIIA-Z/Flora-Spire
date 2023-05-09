#pragma once

#include "engine.h"
#include <SFML\Audio\Music.hpp>
#include <SFML\Graphics\View.hpp>


class MenuScene : public Scene {
private:
	std::vector<sf::Vector2f> posMenuItems;
	std::vector<sf::Vector2f> Picture;
public:
	sf::Music music;
  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void Update(const double& dt) override;
  void Render() override;

  void LoadSave();
};
