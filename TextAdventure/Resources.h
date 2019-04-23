#pragma once
#include "Util.h"
#include <fstream>

class Resources {
public:
	static std::vector<Unit> units;
	static std::vector<Area> areas;
	static std::vector<Tile> tiles;
	static std::vector<Item> items;
	static void LoadUnits();
	static void LoadAreas();
	static void LoadTiles();
	static void LoadItems();
	static Item* GetItem(std::string name = "", std::string rarity = "", std::string type = "", bool randomize = true);
private:
	static Unit CreatePlayer(std::string name);
	static Area CreateArea(std::string name);
};
