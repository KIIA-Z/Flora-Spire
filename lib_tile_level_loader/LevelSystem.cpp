#include "LevelSystem.h"
#include "SFML/Graphics/Texture.hpp"
#include <fstream>

using namespace std;
using namespace sf;

std::map<LevelSystem::Tile, sf::Color> LevelSystem::_colours{
    {WALL, Color::White}, {END, Color::Red}};

sf::Color LevelSystem::getColor(LevelSystem::Tile t) {
  auto it = _colours.find(t);
  if (it == _colours.end()) {
    _colours[t] = Color::Transparent;
  }
  return _colours[t];
}

void LevelSystem::setColor(LevelSystem::Tile t, sf::Color c) {
  _colours[t] = c;
}

std::unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;

float LevelSystem::_tileSize(100.f);
Vector2f LevelSystem::_offset(0.0f, 30.0f);
vector<tInfo> LevelSystem::_sprites;


void LevelSystem::loadLevelFile(const std::string& path, float tileSize) {
  _tileSize = tileSize;
  size_t w = 0, h = 0;
  string buffer;

  // Load in file to buffer
  ifstream f(path);
  if (f.good()) {
    f.seekg(0, std::ios::end);
    buffer.resize(f.tellg());
    f.seekg(0);
    f.read(&buffer[0], buffer.size());
    f.close();
  } else {
      cout << "Couldn't open level file: " + path << endl;
    throw string("Couldn't open level file: ") + path;
  }

  std::vector<Tile> temp_tiles;
  int widthCheck = 0;
  for (int i = 0; i < buffer.size(); ++i) {
    const char c = buffer[i];
    if (c == '\0') {  break; }
    if (c == '\n') { // newline
      if (w == 0) {  // if we haven't written width yet
        w = i;       // set width
      } else if (w != (widthCheck - 1)) {
        throw string("non uniform width:" + to_string(h) + " ") + path;
      }
      widthCheck = 0;
      h++; // increment height
    } else {
      temp_tiles.push_back((Tile)c);
    }
    ++widthCheck;
  }

  if (temp_tiles.size() != (w * h)) {
    throw string("Can't parse level file") + path;
  }
  _tiles = std::make_unique<Tile[]>(w * h);
  _width = w; // set static class vars
  _height = h;
  std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
  cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
  buildSprites();
}

void LevelSystem::buildSprites() {

    _sprites.clear();

    for (size_t y = 0; y < LevelSystem::getHeight(); ++y) {
        for (size_t x = 0; x < LevelSystem::getWidth(); ++x) {

            tInfo i;
            i.pos = (getTilePosition({ x, y }));

            ls::Tile currTile = getTile({ x, y });
            switch (currTile) {

            case WALL:
                i.type = 1;
                _sprites.push_back(move(i));
                break;

            case BLADE:
                i.type = 2;
                _sprites.push_back(move(i));
                break;

            case PIKE:
                i.type = 3;
                _sprites.push_back(move(i));
                break;

            case ROOFSPIKE:
                i.type = 4;
                _sprites.push_back(move(i));
                break;

            case END:
                i.type = 5;
                _sprites.push_back(move(i));
                break;

            case START:
                i.type = 6;
                _sprites.push_back(move(i));
                break;

            default:
                break;

            }
        }
    }
}

//void LevelSystem::render(RenderWindow& window) {
//  for (auto& t : _sprites) {
//    window.draw(*t);
//  }
//}

LevelSystem::Tile LevelSystem::getTile(sf::Vector2ul p) {
  if (p.x > _width || p.y > _height) {
    throw string("Tile out of range: ") + to_string(p.x) + "," +
        to_string(p.y) + ")";
  }
  return _tiles[(p.y * _width) + p.x];
}

size_t LevelSystem::getWidth() { return _width; }

size_t LevelSystem::getHeight() { return _height; }

sf::Vector2f LevelSystem::getTilePosition(sf::Vector2ul p) {
  return (Vector2f(p.x, p.y) * _tileSize) + _offset;
}

std::vector<sf::Vector2ul> LevelSystem::findTiles(LevelSystem::Tile type) {
  auto v = vector<sf::Vector2ul>();
  for (size_t i = 0; i < _width * _height; ++i) {
    if (_tiles[i] == type) {
      v.push_back({i % _width, i / _width});
    }
  }

  return v;
}

LevelSystem::Tile LevelSystem::getTileAt(Vector2f v) {
  auto a = v - _offset;
  if (a.x < 0 || a.y < 0) {
    throw string("Tile out of range ");
  }
  return getTile(Vector2ul((v - _offset) / (_tileSize)));
}

bool LevelSystem::isOnGrid(sf::Vector2f v) {
  auto a = v - _offset;
  if (a.x < 0 || a.y < 0) {
    return false;
  }
  auto p = Vector2ul((v - _offset) / (_tileSize));
  if (p.x > _width || p.y > _height) {
    return false;
  }
  return true;
}

void LevelSystem::setOffset(const Vector2f& _offset) {
  LevelSystem::_offset = _offset;
  buildSprites();
}

void LevelSystem::unload() {
  cout << "LevelSystem unloading\n";
  _sprites.clear();
  _tiles.reset();
  _width = 0;
  _height = 0;
  _offset = {0, 0};
}

const Vector2f& LevelSystem::getOffset() { return _offset; }

float LevelSystem::getTileSize() { return _tileSize; }
