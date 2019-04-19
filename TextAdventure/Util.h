#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#define AreaSize 100
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
	std::string name, equipType;
	Stats use{ 0,0,0,0,0,0 }, equip{ 0,0,0,0,0,0 };
	int value = 0;
};

struct Tile {
	std::string fluff;
	std::vector<Item> items;
	std::vector<unsigned int> units;
	bool seen = false, walkable = true;
};

struct Location {
	int x, y;
	Area* area;
	Tile* tile;
};

struct Area {
	std::string name;
	Tile tiles[AreaSize];
	Tile defaultTile;
};

struct Unit {
	std::string name;
	Stats max;
	Stats cur;
	std::vector<Item> inventory;
	Location loc;
	char type;
};