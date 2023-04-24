#include "engine.h"
#include "game.h"

using namespace std;

MenuScene menu;
SettingsScene settings;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;

int main() {
  Engine::Start(1280, 720, "Flora Spire! ",&menu);
}