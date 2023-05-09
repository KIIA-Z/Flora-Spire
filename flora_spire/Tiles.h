#pragma once
#include <ecm.h>
#include "game.h"

#include "Tile_Manager.h"
#include <SFML/Graphics.hpp>

class Tiles : public Tile {
protected:

    int _tType;

public:
    Tiles(int type, sf::Vector2f pos);
    int getType();
    
    void Update(const float& dt) override;
    void Render(sf::RenderWindow& window) override;

};