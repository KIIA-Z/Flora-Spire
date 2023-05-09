#pragma once

#include "engine.h"
#include "../animation.h"
#include <SFML\Audio\Music.hpp>
#include <SFML\Graphics\View.hpp>

class Level5Scene : public Scene {
public:
  sf::Music music;
  //std::shared_ptr<PlayerSprite> playerSprite;

  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

};
