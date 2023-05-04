#pragma once

#include "scenes/scene_level1.h"
#include "scenes/scene_level2.h"
#include "scenes/scene_level3.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_options.h"

#include <SFML\Audio\Sound.hpp>
#include <SFML\Audio\SoundBuffer.hpp>
#include <SFML\Graphics\Rect.hpp>


extern bool isPaused;

//extern Keyboard::Key attackKey;
//extern Keyboard::Key jumpKey;
//extern Keyboard::Key runKey;


extern MenuScene menu;
extern OptionsScene settings;
extern Level1Scene level1;
extern Level2Scene level2;
extern Level3Scene level3;

//sf::Sound jump;
//sf::Sound walk;
//sf::Sound hurt;
//sf::Sound attack;

