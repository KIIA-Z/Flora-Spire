#include "cmp_melee.h"
using namespace std;
using namespace sf;

void MeleeComponent::update(double dt) {
  _lifetime -= dt;
  if (_lifetime <= 0.f) {
    _parent->setForDelete();
  }
}

MeleeComponent::MeleeComponent(Entity* p, float lifetime)
    : Component(p), _lifetime(0.2) {}
