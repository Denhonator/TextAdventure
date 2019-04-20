#include "Resources.h"

std::vector<Unit> Resources::units;
std::vector<Area> Resources::areas;
std::vector<Tile> Resources::tiles;
std::vector<Item> Resources::items;

void Resources::LoadUnits()
{
	units.push_back(CreatePlayer("player"));
}

void Resources::LoadAreas()
{
	areas.push_back(CreateArea("test"));
}

void Resources::LoadTiles()
{
	tiles.push_back(Tile{ "This is a default tile, edge of the area.",false });
	tiles.push_back(Tile{ "Noting noteworthy to see here.",true });
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