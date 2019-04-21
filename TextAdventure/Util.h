#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

constexpr auto AreaSize = 100;
constexpr auto ItemIndex = 13;
constexpr auto StatIndex = 5;
#define tstr std::to_string

struct Location;
struct Area;
struct Unit;

struct Stats {
	int hp, mp, strength, magic, agility, defense;
};

struct Item {
	std::string name, fluff, type;
	Stats stats { 0,0,0,0,0,0 };
	int value = 0;
};

struct Tile {
	std::string fluff;
	bool walkable = true;
};

struct Location {
	int x, y;
	unsigned int area;
	bool operator==(Location a) {
		return a.x == x && a.y == y && a.area == area;
	}
};

struct Area {
	std::string name;
	unsigned int defaultTile{ 0 };
	unsigned int tiles[AreaSize];
	bool seenTiles[AreaSize];
};

struct Unit {
	std::string name;
	Stats stats{ 20,20,20,20,20,20 };
	Location loc{ 0,0,0 };
	char type = 'p';
	std::vector<unsigned int> inventory;
};