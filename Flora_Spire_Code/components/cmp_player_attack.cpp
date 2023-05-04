#include "cmp_player_attack.h"
#include "cmp_melee.h"
#include "cmp_player_attack.h"
#include "cmp_hurt_player.h"
#include "engine.h"
#include <SFML/Graphics/CircleShape.hpp>
using namespace std;
using namespace sf;

void PlayerAttackComponent::update(double dt) {
    float countdown = 0;
    countdown -= dt;

    if (Keyboard::isKeyPressed(Keyboard::E) && countdown <=0) {
    attack(_parent->scene->direction);
    countdown = 0.75;
  }
    if (sf::Joystick::isButtonPressed(0, 1) && countdown <= 0) {
        attack(_parent->scene->direction);                                                      
        countdown = 0.75;
    }
}

void PlayerAttackComponent::attack(bool direction) const {

  auto attack = _parent->scene->makeEntity();
  if (direction == true) {
      attack->setPosition(_parent->getPosition() + sf::Vector2f(25.f, 0.f));
  }
  else if (direction == false) {
      attack->setPosition(_parent->getPosition() + sf::Vector2f(-30.f, 0.f));
  }
  attack->addComponent<MeleeComponent>();
  auto s = attack->addComponent<ShapeComponent>();
  s->setShape<sf::CircleShape>(10.f);
  s->getShape().setFillColor(Color::Blue);
  s->getShape().setOrigin(Vector2f(8.f, 8.f));
}


PlayerAttackComponent::PlayerAttackComponent(Entity* p)
    : Component(p) {}
