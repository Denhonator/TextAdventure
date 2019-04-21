#include "Resources.h"
#define StatStream st.hp>>st.mp>>st.strength>>st.magic>>st.agility>>st.defense

std::vector<Unit> Resources::units;
std::vector<Area> Resources::areas;
std::vector<Tile> Resources::tiles;
std::vector<Item> Resources::items;

void Resources::LoadUnits()
{
	std::ifstream file;
	Unit u;
	Stats st;
	file.open("Resources/units.txt");
	if (file) {
		while (file >> u.name >> StatStream >> u.loc.x >> u.loc.y >> u.loc.area >> u.type) {
			int item;
			while (file >> item)
				u.inventory.push_back(item);
			u.stats = st;
			units.push_back(u);
		}
	}
	else
		std::cout << "Failed to read Resources/units.txt" << std::endl;
}

void Resources::LoadAreas()
{
	areas.push_back(CreateArea("test"));
}

void Resources::LoadTiles()
{
	std::ifstream file;
	Tile t;
	file.open("Resources/tiles.txt");
	if (file) {
		while (file >> t.fluff >> t.walkable) {
			std::replace(t.fluff.begin(), t.fluff.end(), '_', ' ');
			tiles.push_back(t);
		}
	}
	else
		std::cout << "Failed to read Resources/tiles.txt" << std::endl;
}

void Resources::LoadItems()
{
	std::ifstream file;
	Item i;
	Stats st, st2;
	file.open("Resources/items.txt");
	if (file) {
		while (file >> i.name >> i.fluff >> i.type >> StatStream >> i.value) {
			i.stats = st;
			items.push_back(i);
		}
	}
	else
		std::cout << "Failed to read Resources/items.txt" << std::endl;
}

Area Resources::CreateArea(std::string name) {
	Area a;
	a.name = name;
	for (unsigned int i = 0; i < AreaSize; i++) {
		a.tiles[i] = 1;
		a.seenTiles[i] = false;
	}
	a.defaultTile = 0;
	return a;
}

Unit Resources::CreatePlayer(std::string name) {
	Unit u;
	u.name = name;
	u.stats.hp = 20;
	u.stats.mp = 20;
	u.stats.strength = 20;
	u.stats.magic = 20;
	u.stats.agility = 20;
	u.stats.defense = 20;
	u.type = 'p';
	return u;
}