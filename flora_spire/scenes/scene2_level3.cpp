#include "scene2_level3.h"
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

Save save2_3;

sf::Sprite BackgroundSprite2_3;
sf::Texture BackgroundTexture2_3;

static shared_ptr<Entity> player2_3;
static shared_ptr<Texture> p_texture2_3;
sf::RectangleShape square2_3(sf::Vector2f({ 100.0f,150.0f }));

TileManager tileManager2_3;
std::vector<shared_ptr<Tile>> tiles2_3;

static float countdown = 0.0f;

void Level2_3Scene::Load() {
  //cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/levels/2_3.txt", 40.0f);

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
  if (!BackgroundTexture2_3.loadFromFile("res/backgrounds/area_2_bg.png")) {
      cerr << "Failed to load spritesheet!" << std::endl;
  }

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
      tiles2_3.push_back(tile);
      tileManager2_3.list.push_back(tile);
  }
  

  // Create player
  { 
    player2_3 = makeEntity();
    player2_3->addTag("player");
    p.loadFromFile("res/sprites/playerRight.png");
    player2_3->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
    auto s = player2_3->addComponent<ShapeComponent>();
    s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
    //s->getShape().setFillColor(Color::Magenta);
    s->getShape().setTexture(&p);
    s->getShape().setOrigin(Vector2f(10.f, 15.f));
    player2_3->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
    player2_3->addComponent<PlayerAttackComponent>();
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

void Level2_3Scene::UnLoad() {
  cerr << "Scene 1 Unload" << endl;
  player2_3.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level2_3Scene::Update(const double& dt) {

    Scene::Update(dt);

    if (ls::getTileAt(player2_3->getPosition()) == ls::END) {
        Engine::ChangeScene((Scene*)&level2_4);
    }
    else if (ls::getTileAt(player2_3->getPosition()) == ls::PIKE) {
        player2_3->setForDelete();
        death.play();
    }
    else if (ls::getTileAt(player2_3->getPosition()) == ls::ROOFSPIKE) {
        player2_3->setForDelete();
        death.play();
    }

  if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
      save2_3.UpdateSave(9);
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

void Level2_3Scene::Render() {
  tileRenderer::initialise(Engine::GetWindow());
  tileManager2_3.render(Engine::GetWindow());
  tileRenderer::render();
  Scene::Render();
}
