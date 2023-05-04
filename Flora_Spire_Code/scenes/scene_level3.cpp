#include "scene_level3.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_player_attack.h"
#include "../game.h"
#include "../components/cmp_bullet.h"
#include <LevelSystem.h>
#include <iostream>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void Level3Scene::Load() {
    cout << "Scene 3 Load" << endl;
    ls::loadLevelFile("res/levels/level_3.txt", 40.0f);
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

        cout << " Scene 3 Load Done" << endl;
        setLoaded(true);
    }
}

void Level3Scene::UnLoad() {
  cout << "Scene 3 UnLoad" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level3Scene::Update(const double& dt) {
    Scene::Update(dt);

    const auto pp = player->getPosition();
    if (ls::getTileAt(pp) == ls::END) {
        level1.music.stop();
        Engine::ChangeScene((Scene*)&menu);
    }
    else if (!player->isAlive()) {
        Engine::ChangeScene((Scene*)&level3);
    }

    static float rocktime = 0.0f;
    rocktime -= dt;

    if (rocktime <= 0.f) {
        rocktime = 5.f;
        auto rock = makeEntity();
        rock->setPosition(ls::getTilePosition(ls::findTiles('r')[0]) +
            Vector2f(0, 40));
        rock->addComponent<BulletComponent>(30.f);
        auto s = rock->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(40.f);
        s->getShape().setFillColor(Color::Cyan);
        s->getShape().setOrigin(Vector2f(40.f, 40.f));
        auto p = rock->addComponent<PhysicsComponent>(true, Vector2f(75.f, 75.f));
        p->setRestitution(.4f);
        p->setFriction(.0001f);
        p->impulse(Vector2f(-3.f, 0));
        p->setMass(1000000000.f);
    }

    static float countdown = 0.0f;
    countdown -= dt;

    if ((sf::Keyboard::isKeyPressed(Keyboard::R))) {
        if (countdown <= 0) {
            countdown = 0.2f;
            Engine::ChangeScene((Scene*)Engine::getActiveScene());
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }

    }
}

void Level3Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
