#include "../components/cmp_player_physics.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_player_attack.h"
#include "../game.h"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics.hpp"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void Level2Scene::Load() {
    cout << " Scene 1 Load" << endl;
    ls::loadLevelFile("res/levels/level_2.txt", 40.0f);

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(0, ho));

    // Create player
    {
        player = makeEntity();
        player->addTag("player");
        p.loadFromFile("res/sprites/playerRight.png");
        player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
        auto s = player->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
        //s->getShape().setFillColor(Color::Magenta);
        s->getShape().setTexture(&p);
        s->getShape().setOrigin(Vector2f(10.f, 15.f));
        player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
        player->addComponent<PlayerAttackComponent>();
    }

    // Add physics colliders to level tiles.
    {
        auto walls = ls::findTiles(ls::WALL);
        for (auto w : walls) {
            auto pos = ls::getTilePosition(w);
            pos += Vector2f(20.f, 20.f); //offset to center
            auto e = makeEntity();
            e->setPosition(pos);
            e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
        }
    }

    //Simulate long loading times
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    cout << " Scene 1 Load Done" << endl;

    setLoaded(true);
}

void Level2Scene::UnLoad() {
    cout << "Scene 2 Unload" << endl;
    player.reset();
    ls::unload();
    Scene::UnLoad();
}

void Level2Scene::Update(const double& dt) {

    if (ls::getTileAt(player->getPosition()) == ls::END) {
        Engine::ChangeScene((Scene*)&level3);
    }
    Scene::Update(dt);
    if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
        Engine::GetWindow().close();
    }


    countdown -= dt;

    if (sf::Keyboard::isKeyPressed(Keyboard::Up) && countdown <= 0) {
        countdown = 0.5f;
        jump.play();
    }

    if (sf::Keyboard::isKeyPressed(Keyboard::Left)) {
        p.loadFromFile("res/sprites/playerLeft.png");
    }
    if (sf::Keyboard::isKeyPressed(Keyboard::Right)) {
        p.loadFromFile("res/sprites/playerRight.png");
    }

    if ((sf::Keyboard::isKeyPressed(Keyboard::R))) {
        if (countdown <= 0) {
            countdown = 0.2f;
            Engine::ChangeScene((Scene*)Engine::getActiveScene());
            std::this_thread::sleep_for(std::chrono::milliseconds(1250));
        }

    }
}

void Level2Scene::Render() {
    ls::render(Engine::GetWindow());
    Scene::Render();
}

