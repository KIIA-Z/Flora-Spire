#include "scene_options.h"
#include "../components/cmp_text.h"
#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_text.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_blinking.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_text_list.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include <SFML\Graphics\View.hpp>
#include <SFML\Audio\Music.hpp>
#include "system_renderer.h"
#include "system_physics.h"

using namespace std;
using namespace sf;


static shared_ptr<Entity> resolution;
static shared_ptr<Entity> fullscreen;

void OptionsScene::Load()
{

	auto resolution_text = makeEntity();
	auto t = resolution_text->addComponent<TextComponent>(
		"Resolution:");
	resolution_text->setPosition(Vector2f(200.f, 200.f));

	resolution = makeEntity();
	vector<string> textlist1{ "800 x 600", "1000 x 800", "1024 x 800", "1280 x 1024", "1360 x 768", "1600 x 900", "1920 x 1080" };
	auto t2 = resolution->addComponent<TextComponentList>(textlist1);
	t2->setDefault(to_string(static_cast<int>(Engine::user_preferences.video_resolution.x)) + " x " + to_string(static_cast<int>(Engine::user_preferences.video_resolution.y)));
	resolution->setPosition(Vector2f(600.0f, 200.0f));

	auto fullscreen_text = makeEntity();
	t = fullscreen_text->addComponent<TextComponent>(
		"Full Screen:");
	fullscreen_text->setPosition(Vector2f(200.f, 300.f));

	fullscreen = makeEntity();
	vector<string> textlist{ "ON", "OFF" };
	auto t1 = fullscreen->addComponent<TextComponentList>(textlist);
	if (Engine::user_preferences.fullscreen == 7)
		t1->setDefault("OFF");
	else
		t1->setDefault("ON");
	fullscreen->setPosition(Vector2f(600.0f, 300.0f));

	auto exit = makeEntity();
	t = exit->addComponent<TextComponent>("Return To Menu Menu");
	exit->setPosition(Vector2f(200.f, 500.f));

	auto pointer = makeEntity();
	pointer->addTag("pointer");
	pointer->addComponent<BlinkComponent>(0.5f);
	auto s = pointer->addComponent<ShapeComponent>();
	//float fontsize = t->GetFontSize();
	//hardcoding fontsize atm, because it cannot be accessed from the Update
	pointer->setPosition(resolution_text->getPosition() - Vector2f(20.f, -(20.f))); //Vector2f(20.f, - (fontsize + fontsize / 3) / 2) the box around the text is bigger around 1/3 of fontsize. This will set blinking element central
	s->setShape<sf::RectangleShape>(Vector2f(20.f, 10.f));
	s->getShape().setFillColor(Color::Blue);
	s->getShape().setOrigin({ 5.f, 5.f });

	posMenuItems.push_back(resolution_text->getPosition());
	posMenuItems.push_back(fullscreen_text->getPosition());
	posMenuItems.push_back(exit->getPosition());
	for (int i = 0; i < posMenuItems.size(); i++) {
		cout << posMenuItems[i] << endl;
	}
	pointer_pos = pointer->getPosition() - sf::Vector2f(-420.f, 20.f);


	cout << "Resolution"<< resolution->getPosition() << endl;
	cout << "Fullscreen" << fullscreen->getPosition() << endl;
	cout << pointer->getPosition() << endl;

	setLoaded(true);
}

void OptionsScene::Update(const double& dt)
{
	static int posPoint = 0;
	static float countdown = 1.0f;
	countdown -= dt;

	if (posPoint < 0) {
		posPoint = 2;
		ents.find("pointer")[0]->setPosition(posMenuItems[posPoint] - Vector2f(20.f, -20.f));
	}

	if (posPoint > 2) {
		posPoint = 0;
		ents.find("pointer")[0]->setPosition(posMenuItems[posPoint] - Vector2f(20.f, -20.f));
	}

	if (sf::Keyboard::isKeyPressed(Keyboard::Enter) || sf::Keyboard::isKeyPressed(Keyboard::E) || sf::Keyboard::isKeyPressed(Keyboard::Space)) {
		
		if (ents.find("pointer")[0]->getPosition() == posMenuItems[0] - Vector2f(20.f, -20.f)) {   //change resolution
			string text = resolution->get_components<TextComponentList>()[0]->getText().getString();
			int index = text.find('x');
			int i1;
			int i2;
			stringstream(text.substr(0, index - 1)) >> i1;
			stringstream(text.substr(index + 1)) >> i2;

			Engine::user_preferences.video_resolution = Vector2f(i1, i2);
			Engine::user_preferences.changed_resolution = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			//Engine::ChangeResolution(i1,i2,i1,i2);
			

		}

		else if (ents.find("pointer")[0]->getPosition() == posMenuItems[1] - Vector2f(20.f, -20.f)) {//fullscreen
			
			Engine::user_preferences.changed_fullscreen = true;
			if (fullscreen->get_components<TextComponentList>()[0]->getText().getString() == "ON") {
				Engine::user_preferences.fullscreen = 8;
				cout << "ON" << endl;
				//Engine::SetFullScreen(Engine::GetWindow(), true);
			}
			else {//must be "OFF" then
				
				Engine::user_preferences.fullscreen = 7;
				cout << "OFF" << endl;
				//Engine::SetFullScreen(Engine::GetWindow(),false);
				Engine::getWindowSize();
				//Engine::ChangeResolution();

			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		else if (ents.find("pointer")[0]->getPosition() == posMenuItems[2] - Vector2f(20.f, -20.f)) {//exit
			Engine::ChangeScene(&menu);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}

	if (sf::Keyboard::isKeyPressed(Keyboard::Down)) {
		if (posPoint < (posMenuItems.size() - 1) && countdown <= 0) {
			countdown = 0.15f;//using countdown to give time the player to press again (or will get in this if too many times, pc is fast!)
			posPoint++;
			cout << posPoint << endl;
			cout << posMenuItems[posPoint] << endl;
			ents.find("pointer")[0]->setPosition(posMenuItems[posPoint] - Vector2f(20.f, -20.f));
			pointer_pos = ents.find("pointer")[0]->getPosition();
			cout << pointer_pos - sf::Vector2f(-420.f, 20.f) << endl;
			cout << resolution->getPosition() << endl;
		}
	}

	if (sf::Keyboard::isKeyPressed(Keyboard::Up) || sf::Joystick::isButtonPressed(0, 10)) {
		if (posPoint > 0 && countdown <= 0) {
			countdown = 0.2f;
			posPoint--;
			//pointer_pos = pointer->getPosition();
			cout << posPoint << endl;
			cout << posMenuItems[posPoint] << endl;
			ents.find("pointer")[0]->setPosition(posMenuItems[posPoint] - Vector2f(20.f, -20.f));
			ents.find("pointer")[0]->setPosition(posMenuItems[posPoint] - Vector2f(20.f, -20.f));
			pointer_pos = ents.find("pointer")[0]->getPosition();
			cout << pointer_pos - sf::Vector2f(-420.f, 20.f) << endl;
			cout << resolution->getPosition() << endl;
		}
	}

	if (resolution->get_components<TextComponentList>()[0]->changed)
	{
		string text = resolution->get_components<TextComponentList>()[0]->getText().getString();
		int index = text.find('x');
		int i1;
		int i2;
		stringstream(text.substr(0, index - 1)) >> i1;
		stringstream(text.substr(index + 1)) >> i2;

		Engine::user_preferences.video_resolution = Vector2f(i1, i2);
		Engine::user_preferences.changed_resolution = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::ChangeScene((Scene*)&menu);
	}

	if (fullscreen->get_components<TextComponentList>()[0]->changed) {
		Engine::user_preferences.changed_fullscreen = true;
		if (fullscreen->get_components<TextComponentList>()[0]->getText().getString() == "ON") {
			Engine::user_preferences.fullscreen = 8;
			cout << "ON" << endl;
		}
		else {//must be "OFF" then
			cout << "OFF" << endl;
			Engine::user_preferences.fullscreen = 7;

		}
	}
	Scene::Update(dt);
}
