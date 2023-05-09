#include "scene_victory.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void VictoryScene::Load() {
  cout << "Menu Load \n";
  {
    auto txt = makeEntity();
    auto t = txt->addComponent<TextComponent>(
        "Congratulations!!!\nYou have completed the demo of Flora Sprie\nWe hope you enjoyed\nPress enter  to return to the main menu!");
  }
  setLoaded(true);
}

void VictoryScene::Update(const double& dt) {
  // cout << "Menu Update "<<dt<<"\n";

  if (sf::Keyboard::isKeyPressed(Keyboard::Enter) || sf::Keyboard::isKeyPressed(Keyboard::E) || sf::Keyboard::isKeyPressed(Keyboard::Space)) {
    Engine::ChangeScene(&menu);
  }

  Scene::Update(dt);
}
