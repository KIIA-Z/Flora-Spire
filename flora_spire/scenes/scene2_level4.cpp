#include "scene2_level2.h"
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

Save save2_4;

sf::Sprite BackgroundSprite2_4;
sf::Texture BackgroundTexture2_4;

static shared_ptr<Entity> player2_4;
static shared_ptr<Texture> p_texture2_4;
sf::RectangleShape square2_4(sf::Vector2f({ 100.0f,150.0f }));

TileManager tileManager2_4;
std::vector<shared_ptr<Tile>> tiles2_4;

static float countdown = 0.0f;

void Level2_4Scene::Load() {
  //cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/levels/2_4.txt", 40.0f);

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
  if (!BackgroundTexture2_4.loadFromFile("res/backgrounds/area_2_bg.png")) {
      cerr << "Failed to load spritesheet!" << std::endl;
  }


  BackgroundSprite2_4.setTexture(BackgroundTexture2_4);
  BackgroundSprite2_4.setScale(Vector2f(2, 2));

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
      tiles2_4.push_back(tile);
      tileManager2_4.list.push_back(tile);
  }
  

  // Create player
  { 
    player2_4 = makeEntity();
    player2_4->addTag("player");
    p.loadFromFile("res/sprites/playerRight.png");
    player2_4->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
    auto s = player2_4->addComponent<ShapeComponent>();
    s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
    //s->getShape().setFillColor(Color::Magenta);
    s->getShape().setTexture(&p);
    s->getShape().setOrigin(Vector2f(10.f, 15.f));
    player2_4->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
    player2_4->addComponent<PlayerAttackComponent>();
  }

  // Create Enemy
  {
      auto enemy = makeEntity();
      enemy->addTag("enemy");
      enemy->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]) +
          Vector2f(0, 24));
      auto s = enemy->addComponent<ShapeComponent>();
      e1.loadFromFile("res/sprites/enemy_1.png");
      s->setShape<sf::RectangleShape>(Vector2f(32, 32));
      s->getShape().setTexture(&e1);
      //s->getShape().setFillColor(Color::Red);
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
      e1.loadFromFile("res/sprites/enemy_1.png");
      s->setShape<sf::RectangleShape>(Vector2f(32, 32));
      s->getShape().setTexture(&e1);
      //s->getShape().setFillColor(Color::Red);
      s->getShape().setOrigin(Vector2f(10.f, 15.f));
      enemy->addComponent<HurtComponent>();
      enemy->addComponent<HurtPlayerComponent>();
      enemy->addComponent<EnemyAIComponent>();
  }

  // Create Enemy
  {
      auto enemy = makeEntity();
      enemy->addTag("enemy");
      enemy->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[2]) +
          Vector2f(0, 24));
      auto s = enemy->addComponent<ShapeComponent>();
      e1.loadFromFile("res/sprites/enemy_1.png");
      s->setShape<sf::RectangleShape>(Vector2f(32, 32));
      s->getShape().setTexture(&e1);
      //s->getShape().setFillColor(Color::Red);
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

void Level2_4Scene::UnLoad() {
  cerr << "Scene 1 Unload" << endl;
  player2_4.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level2_4Scene::Update(const double& dt) {

    Scene::Update(dt);

    if (ls::getTileAt(player2_4->getPosition()) == ls::END) {
        Engine::ChangeScene((Scene*)&victory);
    }
    else if (ls::getTileAt(player2_4->getPosition()) == ls::PIKE) {
        player2_4->setForDelete();
        death.play();
    }
    else if (ls::getTileAt(player2_4->getPosition()) == ls::ROOFSPIKE) {
        player2_4->setForDelete();
        death.play();
    }
    else if (!player2_4->isAlive()) {
        Engine::ChangeScene((Scene*)&level2_4);
    }

  if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
      save2_4.UpdateSave(10);
      Engine::ChangeScene((Scene*)&menu);
      music.pause();
      std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  }
  
  countdown -= dt;

  if (sf::Keyboard::isKeyPressed(Keyboard::Up) && countdown <= 0) {
      countdown = 0.5f;
      jump.play();
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::Left)) {
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

void Level2_4Scene::Render() {
  tileRenderer::initialise(Engine::GetWindow());
  tileManager2_4.render(Engine::GetWindow());
  tileRenderer::render();
  Scene::Render();
}
