#pragma once

#include "scenes/scene_level1.h"
#include "scenes/scene_level2.h"
#include "scenes/scene_level3.h"
#include "scenes/scene_level4.h"
#include "scenes/scene_level5.h"
#include "scenes/scene_level6.h"

#include "scenes/scene2_level1.h"
#include "scenes/scene2_level2.h"
#include "scenes/scene2_level3.h"
#include "scenes/scene2_level4.h"

#include "scenes/scene_victory.h"


#include "scenes/scene_menu.h"
#include "scenes/scene_options.h"

#include <SFML/Audio.hpp>
#include "SFML/Graphics/Texture.hpp"

extern bool isPaused;

//extern Keyboard::Key attackKey;
//extern Keyboard::Key jumpKey;
//extern Keyboard::Key runKey;

//textures 
extern sf::Texture BlockTexture;
extern sf::Texture BladeTexture;
extern sf::Texture SpikeTexture;
extern sf::Texture RoofSpikeTexture;
extern sf::Texture ExitTexture;
extern sf::Texture StartTexture;

//levels
extern MenuScene menu;
extern OptionsScene settings;
extern Level1Scene level1;
extern Level2Scene level2;
extern Level3Scene level3;
extern Level4Scene level4;
extern Level5Scene level5;
extern Level6Scene level6;

extern Level2_1Scene level2_1;
extern Level2_2Scene level2_2;
extern Level2_3Scene level2_3;
extern Level2_4Scene level2_4;

extern VictoryScene victory;