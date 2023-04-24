#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_blinking.h"
#include "SFML/Graphics.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML\Audio\Music.hpp>
#include <iostream>
#include <LevelSystem.h>
//#include "../animatedSprite.hpp"

using namespace std;
using namespace sf;

void MenuScene::Load() {
	{
		auto title = makeEntity();
		auto t = title->addComponent<TextComponent>(
			"Flora Spire");
		auto newGame = makeEntity();
		t = newGame->addComponent<TextComponent>("New Game");
		auto load = makeEntity();
		t = load->addComponent<TextComponent>("Load Game");
		auto options = makeEntity();
		t = options->addComponent<TextComponent>("Options");
		auto exit = makeEntity();
		t = exit->addComponent<TextComponent>(
			"Quit Game");

		title->setPosition(Vector2f(200.f, 100.f));
		newGame->setPosition(Vector2f(200.f, 200.f));
		load->setPosition(Vector2f(200.f, 300.f));
		options->setPosition(Vector2f(200.f, 400.f));
		exit->setPosition(Vector2f(200.f, 500.f));
		//cout << newGame->getPosition() << endl;



		auto pointer = makeEntity();
		pointer->addTag("pointer");
		pointer->addComponent<BlinkComponent>(0.5f);
		auto s = pointer->addComponent<ShapeComponent>();
		//float fontsize = t->GetFontSize();
		//hardcoding fontsize atm, because it cannot be accessed from the Update
		pointer->setPosition(newGame->getPosition() - Vector2f(20.f, -(20.f))); //Vector2f(20.f, - (fontsize + fontsize / 3) / 2) the box around the text is bigger around 1/3 of fontsize. This will set blinking element central
		s->setShape<sf::RectangleShape>(Vector2f(20.f, 10.f));
		s->getShape().setFillColor(Color::Blue);
		s->getShape().setOrigin({ 5.f, 5.f });
		posMenuItems.push_back(newGame->getPosition());
		posMenuItems.push_back(load->getPosition());
		posMenuItems.push_back(options->getPosition());
		posMenuItems.push_back(exit->getPosition());
		for (int i = 0; i < posMenuItems.size(); i++) {
			cout << posMenuItems[i] << endl;
		}
	}
}


void MenuScene::Update(const double& dt) {
   //cout << "Menu Update "<<dt<<"\n";

   static int posPoint = 0;
   static float countdown = 0.0f;
   countdown -= dt;

  if (sf::Keyboard::isKeyPressed(Keyboard::Enter)) {
	  if (ents.find("pointer")[0]->getPosition() == posMenuItems[0] - Vector2f(20.f, -20.f)) {//newgame
		  Engine::ChangeScene(&level1);
	  }
	  else if (ents.find("pointer")[0]->getPosition() == posMenuItems[1] - Vector2f(20.f, -20.f)) {//Load
		  //Engine::ChangeScene(&load);
	  }
	  else if (ents.find("pointer")[0]->getPosition() == posMenuItems[2] - Vector2f(20.f, -20.f)) {//options
		  Engine::ChangeScene(&settings);
	  }
	  else if (ents.find("pointer")[0]->getPosition() == posMenuItems[3] - Vector2f(20.f, -20.f)) {//exit
		  Engine::GetWindow().close();
	  }
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::Escape )) {
	  Engine::GetWindow().close();
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::Down)) {
	  if (posPoint < (posMenuItems.size() - 1) && countdown <= 0) {
		  countdown = 0.15f;//using countdown to give time the player to press again (or will get in this if too many times, pc is fast!)
		  posPoint++;
		  cout << posPoint << endl;
		  cout << posMenuItems[posPoint] << endl;
		  ents.find("pointer")[0]->setPosition(posMenuItems[posPoint] - Vector2f(20.f, -20.f));
	  }
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::Up)) {
	  if (posPoint > 0 && countdown <= 0) {
		  countdown = 0.2f;
		  posPoint--;
		  cout << posPoint << endl;
		  cout << posMenuItems[posPoint] << endl;
		  ents.find("pointer")[0]->setPosition(posMenuItems[posPoint] - Vector2f(20.f, -20.f));

	  }
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {


  } // functionality needed for finding mouse click on entity 


  Scene::Update(dt);
}

void MenuScene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}