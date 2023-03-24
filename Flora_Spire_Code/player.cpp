//player.cpp
#include "player.h"
#include "levelSystem.h"

using namespace sf;
using namespace std;

Player::Player()
    : Entity(make_unique<CircleShape>(25.f)), _speed(200.0f)
{
    _shape->setFillColor(Color::Magenta);
    _shape->setOrigin(Vector2f(25.f, 25.f));
}

void Player::Update(double dt) {
    //Move in four directions based on keys
    const Keyboard::Key controls[4] = {
    Keyboard::W, //move up
    Keyboard::S, //move down
    Keyboard::A, //move Left
    Keyboard::D //move Right

    };

    float verticalDirection = 0.0f;
    float horizontalDirection = 0.0f;
    float playerSpeed = 250.f;
    sf::Vector2f shapeDim (12.5f, 12.5f);
    Vector2f tempPos = (_shape->getPosition());

    //needs functionality for diagonal collision, causes weird bug rn

    if (validmove(tempPos) != true && (isKeyPressed(controls[0]) == true))
    {
        verticalDirection--;
        move(Vector2f(0.0f, (verticalDirection * playerSpeed * dt) + 15.f));
    }
    else if (validmove(tempPos) != true && (isKeyPressed(controls[1]) == true))
    {
        verticalDirection++;
        move(Vector2f(0.0f, (verticalDirection * playerSpeed * dt) - 15.f));
    }
    else if (validmove(tempPos) != true && (isKeyPressed(controls[2]) == true))
    {
        horizontalDirection--;
        move(Vector2f((horizontalDirection * playerSpeed * dt) + 15.f, 0.0f));
    }
    else if (validmove(tempPos) != true && (isKeyPressed(controls[3]) == true))
    {
        horizontalDirection++;
        move(Vector2f((horizontalDirection * playerSpeed * dt) - 15.f, 0.0f));
    }
    else {
        if (Keyboard::isKeyPressed(controls[0])) {
            verticalDirection--;
        }
        if (Keyboard::isKeyPressed(controls[1])) {
            verticalDirection++;
        }
        if (Keyboard::isKeyPressed(controls[2])) {
            horizontalDirection--;
        }
        if (Keyboard::isKeyPressed(controls[3])) {
            horizontalDirection++;
        }
        move(Vector2f(horizontalDirection * playerSpeed * dt, 0.f));
        move(Vector2f(0.0f, verticalDirection * playerSpeed * dt));
    }

    Entity::Update(dt);



};

bool Player::validmove(sf::Vector2f pos)
{
    return (ls::getTileAt(pos) != ls::WALL);
}


void Player::Render(sf::RenderWindow& window) const {
    window.draw(*_shape);
}