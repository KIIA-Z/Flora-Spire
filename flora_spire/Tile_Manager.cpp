//entity_management.cpp
#include "Tile_Manager.h"
#include <queue>

using namespace std;
using namespace sf;

// TileManager
void TileManager::update(const float& dt) {
	for (auto& e : list) {
		e->Update(dt);
	}
}

void TileManager::render(sf::RenderWindow& window) {
	for (const auto& e : list) {
		e->Render(window);
	}
}


// Entity Class 
Tile::Tile() {};
void Tile::Update(const float& dt) { }
void Tile::Render(sf::RenderWindow& window) { }


// Tile Renderer
static queue<const Drawable*> sprites;
static RenderWindow* rw;

void tileRenderer::initialise(sf::RenderWindow& r) { rw = &r; }

sf::RenderWindow& tileRenderer::getWindow() { return *rw; }

void tileRenderer::shutdown() {
	while (!sprites.empty())
		sprites.pop();
}

void tileRenderer::update(const double&) {}

void tileRenderer::render() {
	if (rw == nullptr) {
		throw("Render window not set");
	}
	while (!sprites.empty()) {
		rw->draw(*sprites.front());
		sprites.pop();
	}
}

void tileRenderer::queue(const sf::Drawable* s) { sprites.push(s); }
