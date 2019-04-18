#pragma once
#include <vector>
#include <string>
#include <iostream>

#define AreaSize 100

struct Location;
struct Area;
struct Unit;

struct Stats {
	int hp, mp, strength, magic, agility, defense;
};

struct Item {
	std::string name, equipType;
	Stats use, equip;
};

struct Tile {
	std::string fluff;
	std::vector<Item> items;
	std::vector<Unit> enemies;
	bool seen, walkable;
};

struct Location {
	int x, y;
	Area* area;
	Tile* tile;
};

struct Area {
	std::string name;
	Tile tiles[AreaSize];
};

struct Unit {
	std::string name;
	Stats max;
	Stats cur;
	std::vector<Item> inventory;
	Location loc;
};