#include "Tiles.h"
#include "game.h"

using namespace sf;
using namespace std;

Tiles::Tiles(int type, sf::Vector2f pos) : Tile() {

	_tType = type;

	switch (type) {

	case 1:
		// if normal block
		setTexture(BlockTexture);
		break;

	case 2:
		// if break block
		setTexture(BladeTexture);
		break;

	case 3:
		// if break block
		setTexture(SpikeTexture);
		break;

	case 4:
		// if spike
		setTexture(RoofSpikeTexture);
		break;

	case 5:
		// if end
		setTexture(ExitTexture);
		break;

	default:
		break;
	}

	setTextureRect(IntRect(Vector2(0, 0), Vector2(39, 39)));
	setPosition(pos);
};

void Tiles::Update(const float& dt) {
	Tile::Update(dt);
}

int Tiles::getType() {
	return _tType;
}

void Tiles::Render(sf::RenderWindow& window)
{
	tileRenderer::queue(this);
}