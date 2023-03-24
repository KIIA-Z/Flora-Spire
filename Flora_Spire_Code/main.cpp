#include <SFML/Graphics.hpp>
#include <string>
#include "player.h"
#include "game.h"
#include "LevelSystem.h"



using namespace sf;
using namespace std;

std::vector<Player*>  players;
shared_ptr<Player> player = std::make_shared<Player>(sf::Vector2f(100.f, 100.f));

void Load() {
    static Clock clock;
    float dt = clock.restart().asSeconds();

    ls::loadLevelFile("res/levels/maze.txt");

    // Print the level to the console
    for (size_t y = 0; y < ls::getHeight(); ++y) {
        for (size_t x = 0; x < ls::getWidth(); ++x) {
          cout << ls::getTile({ x, y });
        }
       cout << endl;
    }
};

void Update(RenderWindow& window) {
    static Clock clock;
    float dt = clock.restart().asSeconds();
    Event event;

    player->Update(dt);

    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }
};

void Render(RenderWindow& window) {
    player->Render(window);

};

int main() {
    
    RenderWindow window(VideoMode({ gameWidth, gameHeight }), "Flora Spire Dev");
    Load();
    while (window.isOpen()) {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}


