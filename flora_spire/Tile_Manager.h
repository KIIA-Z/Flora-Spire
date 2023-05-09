//entity_management.h
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Tile : public sf::Sprite {
protected:
    Tile();
public:

    virtual ~Tile() = default;

    virtual void Update(const float& dt);
    virtual void Render(sf::RenderWindow& window);
};

struct TileManager {
    std::vector < std::shared_ptr < Tile >> list;
    void update(const float& dt);
    void render(sf::RenderWindow& window);
};

namespace tileRenderer {
    void initialise(sf::RenderWindow&);
    sf::RenderWindow& getWindow();

    void shutdown();
    void update(const double&);
    void queue(const sf::Drawable* s);
    void render();
};