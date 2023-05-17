#include "cmp_hurt_player.h"
#include <engine.h>

using namespace std;
using namespace sf;

void HurtComponent::update(double dt) {

    //checks to make sure player is attacking before being allowed to kill enemy
    if (_parent->scene->attacking == true) {
        if (auto pl = _player.lock()) {

            if (length(pl->getPosition() - _parent->getPosition()) < 50.0) {
                _parent->setForDelete();
            }
        }
    } 
}

HurtComponent::HurtComponent(Entity* p)
    : Component(p) , _player(_parent->scene->ents.find("player")[0]) {}


void HurtPlayerComponent::update(double dt) {
    if (auto pl = _player.lock()) {
        if (length(pl->getPosition() - _parent->getPosition()) < 25.0) {
            pl->setForDelete();
            _parent->setForDelete();
        }
    }
}

HurtPlayerComponent::HurtPlayerComponent(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]) {}

