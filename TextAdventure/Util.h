#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <time.h>

struct Location;
struct Unit;

struct Stats {
	int hp, mp, strength, magic, agility, defense;
	Stats operator+(const Stats& s) const
	{
		return Stats{ hp + s.hp,mp + s.mp,strength + s.strength,magic + s.magic,agility + s.agility,defense + s.defense };
	}
};

struct Item {
	std::string name, fluff, type, rarity;
	Stats stats { 0,0,0,0,0,0 };
	int value = 0;
};

struct Tile {
	std::string fluff = "";
	char type = 'a';
	std::vector<Unit> units;
};

struct Unit {
	std::string name = "";
	Stats stats{ 0,0,0,0,0,0 };
	char type = 0;
	std::string equipment[6];
	std::pair<unsigned int, std::string> inventory[6];
};