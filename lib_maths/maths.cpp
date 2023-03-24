\\main.cpp
#include "LevelSystem.h"

...

void load() {
    ...
        ls::loadLevelFile("res/levels/maze.txt");

    // Print the level to the console
    for (size_t y = 0; y < ls::getHeight(); ++y) {
        for (size_t x = 0; x < ls::getWidth(); ++x) {
            cout << ls::getTile({ x, y });
        }
        cout << endl;
    }
}
...
void Render(RenderWindow& window) {
    ls::Render(window);
    ...
}