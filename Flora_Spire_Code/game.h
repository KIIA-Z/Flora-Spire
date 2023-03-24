//game.h
#pragma once
#include <SFML/Graphics.hpp>
#include "player.cpp"

constexpr uint16_t gameWidth = 800;
constexpr uint16_t gameHeight = 600;

extern std::vector<Player*> players;