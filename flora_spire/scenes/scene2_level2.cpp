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

Save save2_2;

sf::Sprite BackgroundSprite2_2;
sf::Texture BackgroundTexture2_2;

static shared_ptr<Entity> player2_2;
static shared_ptr<Texture> p_texture2_2;
static shared_ptr<Texture> background2_2;
sf::RectangleShape square2_2(sf::Vector2f({ 100.0f,150.0f }));

TileManager tileManager2_2;
std::vector<shared_ptr<Tile>> tiles2_2;

static float countdown = 0.0f;

void Level2_2Scene::Load() {
  //cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/levels/2_2.txt", 40.0f);

  auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
  ls::setOffset(Vector2f(0, ho));


  if (!BlockTexture.loadFromFile("res/sprites/grass.png")) {
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
  if (!BackgroundTexture2_2.loadFromFile("res/backgrounds/area_2_bg.png")) {
      cerr << "Failed to load spritesheet!" << std::endl;
  }

  BackgroundSprite2_2.setTexture(BackgroundTexture2_2);
  BackgroundSprite2_2.setScale(Vector2f(2, 2));


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
      tiles2_2.push_back(tile);
      tileManager2_2.list.push_back(tile);
  }
  

  // Create player
  { 
    player2_2 = makeEntity();
    player2_2->addTag("player");
    p.loadFromFile("res/sprites/playerRight.png");
    player2_2->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
    auto s = player2_2->addComponent<ShapeComponent>();
    s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
    //s->getShape().setFillColor(Color::Magenta);
    s->getShape().setTexture(&p);
    s->getShape().setOrigin(Vector2f(10.f, 15.f));
    player2_2->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
    player2_2->addComponent<PlayerAttackComponent>();
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

void Level2_2Scene::UnLoad() {
  cerr << "Scene 1 Unload" << endl;
  player2_2.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level2_2Scene::Update(const double& dt) {

    Scene::Update(dt);

    if (ls::getTileAt(player2_2->getPosition()) == ls::END) {
        Engine::ChangeScene((Scene*)&level2_3);
    }
    else if (ls::getTileAt(player2_2->getPosition()) == ls::PIKE) {
        player2_2->setForDelete();
        death.play();
    }
    else if (ls::getTileAt(player2_2->getPosition()) == ls::ROOFSPIKE) {
        player2_2->setForDelete();
        death.play();
    }


  Scene::Update(dt);
  if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
      save2_2.UpdateSave(8);
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

void Level2_2Scene::Render() {
  tileRenderer::initialise(Engine::GetWindow());
  tileManager2_2.render(Engine::GetWindow());
  tileRenderer::render();
  Scene::Render();
}
