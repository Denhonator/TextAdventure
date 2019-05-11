#pragma once
#include "Util.h"
#include <fstream>

class Resources {
public:
	static std::vector<Unit> units;
	static std::vector<Item> items;
	static void LoadUnits();
	static void LoadItems();
	static Item* GetItem(std::string name = "", std::string rarity = "", std::string type = "", bool randomize = true);
	static Unit GetUnit(std::string name);
	static Encounter GetEncounter(std::string type = "enemy");
private:
	static Unit CreatePlayer(std::string name);
};
