#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "Flora Spire");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(shape);
        window.display();
    }
    return 0;
}

void Update(RenderWindow& window) {
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }
}