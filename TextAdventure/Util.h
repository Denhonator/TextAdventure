#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <time.h>
#include <map>

struct Location;
struct Unit;

struct Stats {
	int hp, mp, strength, magic, agility, defense;
	Stats operator+(const Stats& s) const {
		return Stats{ hp + s.hp,mp + s.mp,strength + s.strength,magic + s.magic,agility + s.agility,defense + s.defense };
	}
};

struct Attribute {
	int physical, fire, ice, lightning, neutral;
	Attribute operator+(const Attribute& a) const {
		return Attribute{ physical + a.physical,fire + a.fire,ice + a.ice,lightning + a.lightning,neutral + a.neutral };
	}
};

struct Item {
	std::string name, fluff, type, rarity;
	Attribute stats { 0,0,0,0,0 };
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
	std::map<std::string, unsigned int> inventory;
};

struct Encounter {
	std::string objective = "";
	Stats stats{ 0,0,0,0,0,0 };
	Attribute resist{ 1,1,1,1,1 };
	Attribute attack{ 0,0,0,0,0 };
};