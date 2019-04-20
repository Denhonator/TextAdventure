#include "Resources.h"
#define stats st.hp>>st.mp>>st.strength>>st.magic>>st.agility>>st.defense
#define stats2 st2.hp>>st2.mp>>st2.strength>>st2.magic>>st2.agility>>st2.defense

std::vector<Unit> Resources::units;
std::vector<Area> Resources::areas;
std::vector<Tile> Resources::tiles;
std::vector<Item> Resources::items;

void Resources::LoadUnits()
{
	std::ifstream file;
	Unit u;
	Stats st, st2;
	file.open("Resources/units.txt");
	if (file) {
		while (file >> u.name >> stats >> stats2 >> u.loc.x >> u.loc.y >> u.loc.area >> u.type) {
			u.cur = st;
			u.max = st2;
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
	u.max.hp = 20;
	u.max.mp = 20;
	u.max.strength = 20;
	u.max.magic = 20;
	u.max.agility = 20;
	u.max.defense = 20;
	u.cur = u.max;
	u.type = 'p';
	return u;
}