#include "engine.h"
#include "game.h"

using namespace std;

MenuScene menu;
OptionsScene settings;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
Level4Scene level4;
Level5Scene level5;
Level6Scene level6;

Level2_1Scene level2_1;
Level2_2Scene level2_2;
Level2_3Scene level2_3;
Level2_4Scene level2_4;

VictoryScene victory;


void load() {
}

int main() {
  Engine::Start(1280, 720, "Flora Spire! ",&menu);
  
  
}

