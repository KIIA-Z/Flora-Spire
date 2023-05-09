#include "scene_menu.h"
#include "scene_level1.h"
#include "scene2_level1.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "../save.h"
#include "../components/cmp_blinking.h"
#include "../components/cmp_sprite.h"
#include "SFML/Graphics.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML\Audio\Music.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>
#include <LevelSystem.h>

using namespace std;
using namespace sf;

sf::Sound pointer;
sf::Sound select;
sf::SoundBuffer pointerBuff;
sf::SoundBuffer selectBuff;

Save save;

sf::Texture treePic;


void MenuScene::Load() {

	Engine::GetWindow().setFramerateLimit(120);
	menu.music.stop();

	if (sf::Joystick::isConnected(0)) {
		cout << "controler is connected" << endl;
	}

	if (!level1.music.openFromFile("res/music/area_1.wav"))
		cout << "Error: could not find music file";
	level1.music.setLoop(true);
	level1.music.setVolume(20);
	if (!level2_1.music.openFromFile("res/music/area_2.wav"))
		cout << "Error: could not find music file";
	level2_1.music.setLoop(true);
	level2_1.music.setVolume(20);
	
	//loads muisc from file then plays music on a loop
	menu.music.stop();
	if (!this->music.openFromFile("res/music/menu_music.wav"))
		cout << "Error: could not find music file";
	this->music.setLoop(true);
	this->music.setVolume(15);
	this->music.play();

	//loads jump sound to be used when player jumps
	pointerBuff.loadFromFile(("res/sounds/pointer.wav"));
	pointer.setBuffer(pointerBuff);

	//loads hurt sound to be used when player is hurt
	selectBuff.loadFromFile(("res/sounds/enter.wav"));
	select.setBuffer(selectBuff);

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

		//menu picture
		{
			auto picture = makeEntity();
			picture->addTag("pointer");
			treePic.loadFromFile("res/img/treePic.png");
			picture->setPosition(Vector2f(500.f, 150.f));
			auto s = picture->addComponent<ShapeComponent>();
			s->setShape<sf::RectangleShape>(Vector2f(500.f, 400.f));
			s->getShape().setTexture(&treePic);
			s->getShape().setOrigin(Vector2f(10.f, 15.f));
			Picture.push_back(picture->getPosition());
		}


		
	}
}			

void MenuScene::Update(const double& dt) {
	if (sf::Joystick::isButtonPressed(0, 1)) {
		cout << " is pressed" << endl;
	}

   static int posPoint = 0;
   static float countdown = 1.0f;
   countdown -= dt;

   if (posPoint < 0) {
	   posPoint = 3;
	   ents.find("pointer")[0]->setPosition(posMenuItems[posPoint] - Vector2f(20.f, -20.f));
   }

   if (posPoint > 3) {
	   posPoint = 0;
	   ents.find("pointer")[0]->setPosition(posMenuItems[posPoint] - Vector2f(20.f, -20.f));
   }

  if (sf::Keyboard::isKeyPressed(Keyboard::Enter) || sf::Keyboard::isKeyPressed(Keyboard::E) || sf::Keyboard::isKeyPressed(Keyboard::Space)) {
	  select.play();
	  if (ents.find("pointer")[0]->getPosition() == posMenuItems[0] - Vector2f(20.f, -20.f)) {//newgame
		  Engine::ChangeScene(&level1);
	  }
	  else if (ents.find("pointer")[0]->getPosition() == posMenuItems[1] - Vector2f(20.f, -20.f)) {//Load
		  LoadSave();
	  }
	  else if (ents.find("pointer")[0]->getPosition() == posMenuItems[2] - Vector2f(20.f, -20.f)) {//options
		  Engine::ChangeScene(&settings);
		  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
		  pointer.play();
		  cout << posPoint << endl;
		  cout << posMenuItems[posPoint] << endl;
		  ents.find("pointer")[0]->setPosition(posMenuItems[posPoint] - Vector2f(20.f, -20.f));
	  }

  }

  if (sf::Keyboard::isKeyPressed(Keyboard::Up) || sf::Joystick::isButtonPressed(0,10)) {
	  if (posPoint > 0 && countdown <= 0) {
		  countdown = 0.2f;
		  posPoint--;
		  pointer.play();
		  cout << posPoint << endl;
		  cout << posMenuItems[posPoint] << endl;
		  ents.find("pointer")[0]->setPosition(posMenuItems[posPoint] - Vector2f(20.f, -20.f));

	  }
	 
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {


  } // functionality needed for detecting mouse click on entity 


  Scene::Update(dt);
}

void MenuScene::Render() {
	//ls::render(Engine::GetWindow());
	Scene::Render();
}

void MenuScene::LoadSave() {
	save.CheckFolder();
	save.CheckSave();
	int level = save.LoadProgress();
	cout << level << endl;
	switch (level) {
	case 1:
		Engine::ChangeScene(&level1);
		music.pause();
		level1.music.play();
		break;
	case 2:
		Engine::ChangeScene(&level2);
		music.pause();
		level1.music.play();
		break;
	case 3:
		Engine::ChangeScene(&level3);
		music.pause();
		level1.music.play();
		break;
	case 4:
		Engine::ChangeScene(&level4);
		music.pause();
		level1.music.play();
		break;
	case 5:
		Engine::ChangeScene(&level5);
		music.pause();
		level1.music.play();
		break;
	case 6:
		Engine::ChangeScene(&level6);
		music.pause();
		level1.music.play();
		break;
	case 7:
		Engine::ChangeScene(&level2_1);
		music.pause();
		level2_1.music.play();
		break;
	case 8:
		Engine::ChangeScene(&level2_2);
		music.pause();
		level2_1.music.play();
		break;
	case 9:
		Engine::ChangeScene(&level2_3);
		music.pause();
		level2_1.music.play();
		break;
	case 10:
		Engine::ChangeScene(&level2_4);
		music.pause();
		level2_1.music.play();
		break;
	default:
		Engine::ChangeScene(&menu);
	}
}