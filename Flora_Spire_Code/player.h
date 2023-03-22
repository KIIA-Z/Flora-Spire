//player.h
#pragma once
#include "entity.h"

class Player : public Entity {
private:
	float _speed;

public:
	void Update(double dt);
	bool validmove(sf::Vector2f pos);
	Player();
	void Render(sf::RenderWindow& window) const;
};