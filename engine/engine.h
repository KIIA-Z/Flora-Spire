#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include "user_preferences.h"
#include <ecm.h>
#include <future>
#include <maths.h>
#include <mutex>
#include <string>

class Scene {
public:
    sf::Music music;
    bool attacking;
  Scene() = default;
  virtual ~Scene();
  virtual void Load() = 0;
  virtual void LoadAsync();
  virtual void UnLoad();
  virtual void Update(const double& dt);
  virtual void Render();
  bool isLoaded() const;
  double countdown;
  bool direction = true;
  std::shared_ptr<Entity> makeEntity();
  std::shared_ptr<sf::Sprite> getBackground();
  void setBackground(sf::Sprite& background);
  sf::Vector2f pointer_pos;
  sf::Texture p;

  //set up global access to varibles that hold player sounds 
  sf::Sound jump;
  sf::Sound death;
  sf::Sound hurt;
  sf::SoundBuffer jumpBuff;
  sf::SoundBuffer deathBuff;
  sf::SoundBuffer hurtBuff;

  //set up global access to varibles that hold player sprites
  sf::Sprite playerRight;
  sf::Sprite playerLeft;

  EntityManager ents;

protected:
  void setLoaded(bool);
  std::shared_ptr<sf::Sprite> _background;
private:
  mutable bool _loaded;
  mutable std::future<void> _loaded_future;
  mutable std::mutex _loaded_mtx;
};

class Engine {
public:
  Engine() = delete;
  static void Start(unsigned int width, unsigned int height,
                    const std::string& gameName, Scene* scn);
  void Engine::restartLevel(float& dt);
  //static void Quit(RenderWindow window);
  static void ChangeScene(Scene*);
  static sf::RenderWindow& GetWindow();
  static sf::Vector2u getWindowSize();
  static void setVsync(bool b);
  static Scene* getActiveScene();
  static UserPreferences user_preferences;

private:
  static Scene* _activeScene;
  static std::string _gameName;
  static void Update();
  static void Render(sf::RenderWindow& window);
};

namespace timing {
// Return time since Epoc
long long now();
// Return time since last() was last called.
long long last();
} // namespace timing