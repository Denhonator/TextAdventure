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
	while (input[input.size() - 1] == ' ')
		input = input.substr(0, input.size() - 1);
	std::string arg = "";
	if (input.find(" ") != std::string::npos) {
		arg = input.substr(input.find(" ")+1);
		input = input.substr(0, input.find(" "));
	}
	if (input == "player")
		return PrintUnit(&players.at(curPlayer));
	if (input == "look") {
		return LookTile(arg);
	}
	return "Invalid command";
}

std::string Game::LookTile(std::string arguments) {
	if (arguments == "") {
		return PrintTile(players.at(curPlayer).loc.tile);
	}
	Location l = players.at(curPlayer).loc;
	bool validArg = false;
	if (arguments == "east") {
		validArg = true;
		l.x += 1;
	}
	else if (arguments == "west") {
		validArg = true;
		l.x -= 1;
	}
	else if (arguments == "north") {
		validArg = true;
		l.y -= 1;
	}
	else if (arguments == "south") {
		validArg = true;
		l.y += 1;
	}
	if (!validArg)
		return "Invalid argument";
	if(l.x>=0&&l.x<10&&l.y>=0&&l.y<10)
		return PrintTile(&l.area->tiles[l.x + l.y * 10]);
	return PrintTile(&l.area->defaultTile);
}

Area CreateArea(std::string name) {
	Area a;
	a.name = name;
	for (unsigned int i = 0; i < AreaSize; i++) {
		a.tiles[i].fluff = "Nothing noteworthy to see.";
		a.tiles[i].seen = false;
		a.tiles[i].walkable = true;
	}
	a.defaultTile.fluff = "This is a default tile, which surround the area.";
	a.defaultTile.seen = true;
	a.defaultTile.walkable = false;
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
	if (tile->fluff.size() > 1) {
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