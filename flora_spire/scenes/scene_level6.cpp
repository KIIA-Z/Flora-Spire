#include "scene_level6.h"
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

Save save6;

sf::Sprite BackgroundSprite6;
sf::Texture BackgroundTexture6;

static shared_ptr<Entity> player6;
static shared_ptr<Texture> p_texture6;
static shared_ptr<Texture> background6;
sf::RectangleShape square6(sf::Vector2f({ 100.0f,150.0f }));

TileManager tileManager6;
std::vector<shared_ptr<Tile>> tiles6;

static float countdown = 0.0f;

void Level6Scene::Load() {
  //cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/levels/1_8.txt", 40.0f);

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
 if (!BackgroundTexture6.loadFromFile("res/backgrounds/area_1_bg.png")) {
     cerr << "Failed to load spritesheet!" << std::endl;
 }

 BackgroundSprite6.setTexture(BackgroundTexture6);
 BackgroundSprite6.setScale(Vector2f(2, 2));

  //loads jump sound to be used when player jumps
  jumpBuff.loadFromFile(("res/sounds/jump.ogg"));
  jump.setBuffer(jumpBuff);

  //loads hurt sound to be used when player is killed
  deathBuff.loadFromFile(("res/sounds/jump.ogg"));
  death.setBuffer(deathBuff);

  // add tiles to tile list
  for (int i = 0; i < ls::_sprites.size(); i++) {

      tInfo curr = ls::_sprites[i];

      std::shared_ptr<Tile> tile = std::make_shared<Tiles>(curr.type, curr.pos);
      tiles6.push_back(tile);
      tileManager6.list.push_back(tile);
  }
  

  // Create player
  { 
    player6 = makeEntity();
    player6->addTag("player");
    p.loadFromFile("res/sprites/playerRight.png");
    player6->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]) - Vector2f(10,10));
    auto s = player6->addComponent<ShapeComponent>();
    s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
    //s->getShape().setFillColor(Color::Magenta);
    s->getShape().setTexture(&p);
    s->getShape().setOrigin(Vector2f(10.f, 15.f));
    player6->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
    player6->addComponent<PlayerAttackComponent>();
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

void Level6Scene::UnLoad() {
  cerr << "Scene 1 Unload" << endl;
  player6.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level6Scene::Update(const double& dt) {
    Scene::Update(dt);

  if (ls::getTileAt(player6->getPosition()) == ls::END) {
    Engine::ChangeScene((Scene*)&level2_1);
  }
  else if (ls::getTileAt(player6->getPosition()) == ls::PIKE) {
      player6->setForDelete();
      death.play();
  }

 
  if (sf::Keyboard::isKeyPressed(Keyboard::Escape)) {
      save6.UpdateSave(6);
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

void Level6Scene::Render() {
  Engine::GetWindow().draw(BackgroundSprite6);
  tileRenderer::initialise(Engine::GetWindow());
  tileManager6.render(Engine::GetWindow());
  tileRenderer::render();
  Scene::Render();
}
