#include "scene_level2.h"
#include "../Tile_Manager.h"
#include "../Tiles.h"
#include "../save.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_player_attack.h"
#include "../game.h"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

sf::Sprite BackgroundSprite2;
sf::Texture BackgroundTexture2;

static shared_ptr<Entity> player2;
static shared_ptr<Texture> p_texture2;
static shared_ptr<Texture> background2;
sf::RectangleShape square2(sf::Vector2f({ 100.0f,150.0f }));

TileManager tileManager2;
std::vector<shared_ptr<Tile>> tiles2;

Save save2;

static float countdown = 0.0f;

void Level2Scene::Load() {
    //cout << " Scene 1 Load" << endl;
    ls::loadLevelFile("res/levels/1_2.txt", 40.0f);

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(0, ho));

    if (!BlockTexture.loadFromFile("res/sprites/wood.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }
    if (!SpikeTexture.loadFromFile("res/sprites/spike.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }
    if (!RoofSpikeTexture.loadFromFile("res/sprites/rspike.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }
    if (!ExitTexture.loadFromFile("res/sprites/exit.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }

    if (!BackgroundTexture2.loadFromFile("res/backgrounds/area_1_bg.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }

    BackgroundSprite2.setTexture(BackgroundTexture2);
    BackgroundSprite2.setScale(Vector2f(2, 2));

    //loads jump sound to be used when player jumps
    jumpBuff.loadFromFile(("res/sounds/jump.ogg"));
    jump.setBuffer(jumpBuff);

    //loads hurt sound to be used when player is hurt
    hurtBuff.loadFromFile(("res/sounds/jump.ogg"));
    hurt.setBuffer(jumpBuff);

    // add tiles to tile list
    for (int i = 0; i < ls::_sprites.size(); i++) {

        tInfo curr = ls::_sprites[i];

        std::shared_ptr<Tile> tile = std::make_shared<Tiles>(curr.type, curr.pos);
        tiles2.push_back(tile);
        tileManager2.list.push_back(tile);
    }

    // Create player
    {
        player2 = makeEntity();
        player2->addTag("player");
        p.loadFromFile("res/sprites/playerRight.png");
        player2->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
        auto s = player2->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
        //s->getShape().setFillColor(Color::Magenta);
        s->getShape().setTexture(&p);
        s->getShape().setOrigin(Vector2f(10.f, 15.f));
        player2->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
        player2->addComponent<PlayerAttackComponent>();
    }

    // Create Enemy
    {
        auto enemy = makeEntity();
        enemy->addTag("enemy");
        enemy->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]) +
            Vector2f(0, 24));
        auto s = enemy->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(16.f);
        s->getShape().setFillColor(Color::Red);
        s->getShape().setOrigin(Vector2f(10.f, 15.f));
        enemy->addComponent<HurtComponent>();
        enemy->addComponent<HurtPlayerComponent>();
        enemy->addComponent<EnemyAIComponent>();
    }

    // Create Enemy
    {
        auto enemy = makeEntity();
        enemy->addTag("enemy");
        enemy->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[1]) +
            Vector2f(0, 24));
        auto s = enemy->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(16.f);
        s->getShape().setFillColor(Color::Red);
        s->getShape().setOrigin(Vector2f(10.f, 15.f));
        enemy->addComponent<HurtComponent>();
        enemy->addComponent<HurtPlayerComponent>();
        enemy->addComponent<EnemyAIComponent>();
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

    //Simulate loading times
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    cerr << " Scene 1 Load Done" << endl;

    setLoaded(true);
}

void Level2Scene::UnLoad() {
    cerr << "Scene 1 Unload" << endl;
    player2.reset();
    ls::unload();
    Scene::UnLoad();
}

void Level2Scene::Update(const double& dt) {
    Scene::Update(dt);
    bool stop;

    if (ls::getTileAt(player2->getPosition()) == ls::END) {
        Engine::ChangeScene((Scene*)&level3);
    }
    else if (ls::getTileAt(player2->getPosition()) == ls::PIKE) {
        player2->setForDelete();
        death.play();
    }
   
    if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
        save2.UpdateSave(2);
        Engine::ChangeScene((Scene*)&menu);
        level1.music.stop();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }



    countdown -= dt;

    if (sf::Keyboard::isKeyPressed(Keyboard::Up) && countdown <= 0) {
        countdown = 0.5f;
        jump.play();
    }

    if (sf::Keyboard::isKeyPressed(Keyboard::Left)) {
        cerr << "direction is false" << endl;
        direction = false;
        p.loadFromFile("res/sprites/playerLeft.png");
    }
    if (sf::Keyboard::isKeyPressed(Keyboard::Right)) {
        direction = true;
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
    Engine::GetWindow().draw(BackgroundSprite2);
    tileRenderer::initialise(Engine::GetWindow());
    tileManager2.render(Engine::GetWindow());
    tileRenderer::render();
    Scene::Render();
}
