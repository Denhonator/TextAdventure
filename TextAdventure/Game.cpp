#include "Game.h"

Area CreateArea(std::string name);
Unit CreatePlayer(std::string name, Location loc);
std::string PrintUnit(Unit* unit);
std::string PrintTile(Tile* tile);

Game::Game()
{
	areas.push_back(CreateArea("test"));
	players.push_back(CreatePlayer("player1", Location{ 0,0,&areas.at(0) }));
}

Game::~Game()
{
}

std::string Game::ProcessCommand(std::string input)
{
	if (input == "player")
		return PrintUnit(&players.at(curPlayer));
	else if (input == "tile")
		return PrintTile(players.at(curPlayer).loc.tile);
	else if (input == "look right") {
		Location l = players.at(curPlayer).loc;
		if (l.x < 9) {
			return PrintTile(&l.area->tiles[l.x + 1 + l.y * 10]);
		}
		else {
			return PrintTile(nullptr);
		}
	}
	return "Invalid command";
}

Area CreateArea(std::string name) {
	Area a;
	a.name = name;
	for (unsigned int i = 0; i < AreaSize; i++) {
		a.tiles[i].fluff = "Nothing noteworthy to see";
		a.tiles[i].seen = false;
		a.tiles[i].walkable = true;
	}
	return a;
}

Unit CreatePlayer(std::string name, Location loc) {
	Unit u;
	u.name = name;
	u.max.hp = 20;
	u.max.mp = 20;
	u.max.strength = 20;
	u.max.magic = 20;
	u.max.agility = 20;
	u.max.defense = 20;
	u.cur = u.max;
	u.loc = loc;
	u.loc.tile = &u.loc.area->tiles[u.loc.x + u.loc.y * 10];
	u.loc.tile->walkable = true;
	u.loc.tile->seen = true;
	return u;
}

std::string PrintUnit(Unit* unit) {
	return "Name: " + unit->name + "\n"
		+ std::to_string(unit->cur.hp) + "/" + std::to_string(unit->max.hp) + " HP"
		+ "      Strength: " + std::to_string(unit->cur.strength) + "\n"
		+ std::to_string(unit->cur.mp) + "/" + std::to_string(unit->max.mp) + " MP"
		+ "      Magic:    " + std::to_string(unit->cur.magic) + "\n"
		+ "              Defense:  " + std::to_string(unit->cur.defense) + "\n"
		+ "              Agility:  " + std::to_string(unit->cur.agility);
}

std::string PrintTile(Tile* tile) {
	if (tile == nullptr)
		return "You can not go this way";
	std::string s = "";
	if (tile->fluff.length() > 1) {
		s = tile->fluff + "\n";
	}
	if (tile->seen) {
		for (unsigned int i = 0; i < tile->enemies.size(); i++) {
			if (i == 0) {
				s += "You see these enemies: ";
			}
			s += "\n" + tile->enemies.at(i).name;
		}
		for (unsigned int i = 0; i < tile->items.size(); i++) {
			if (i == 0) {
				s += "You see these items: ";
			}
			s += "\n" + tile->items.at(i).name;
		}
	}
	else if (tile->walkable) {
		s += "You do not know what you will find here.";
	}
	else {
		s += "You can not go this way.";
	}
	return s;
}