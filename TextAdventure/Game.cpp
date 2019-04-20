#include "Game.h"

Game::Game()
{
	Resources::LoadTiles();
	Resources::LoadAreas();
	Resources::LoadItems();
	Resources::LoadUnits();
	tiles = Resources::tiles;
	areas = Resources::areas;
	items = Resources::items;
	units = Resources::units;
	AddPlayer("player0");
}

Game::~Game()
{
}

std::string Game::ProcessCommand(std::string input, int player)
{
	curPlayer = player;
	while (input[input.size() - 1] == ' ')
		input = input.substr(0, input.size() - 1);
	std::string arg = "";
	if (input.find(" ") != std::string::npos) {
		arg = input.substr(input.find(" ")+1);
		input = input.substr(0, input.find(" "));
	}
	if (input == "player")
		return PrintUnit(&units.at(curPlayer));
	if (input == "look") {
		return LookTile(arg);
	}
	if (input == "move") {
		if(Move(arg, curPlayer))
			return LookTile();
		return LookTile(arg);
	}
	return "Invalid command";
}

void Game::AddPlayer(std::string name, Location loc)
{
	Unit u = Resources::units.at(0);
	u.name = name;
	units.insert(units.begin()+players,u);
	AddUnitTo(loc, players);
	players++;
}

bool Game::AddUnitTo(Location loc, unsigned int index)
{
	if (loc.x >= 0 && loc.x < 10 && loc.y >= 0 && loc.y < 10) {
		units.at(index).loc = loc;
		if (units.at(index).type == 'p')
			areas.at(loc.area).seenTiles[loc.x + loc.y * 10] = true;
		return true;
	}
	return false;
}

std::string Game::LookTile(std::string arguments) {
	Location l = units.at(curPlayer).loc;
	if (arguments == "") {
		return PrintTile(l);
	}
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
	return PrintTile(l);
}

bool Game::Move(std::string arguments, unsigned int index)
{
	Location l = units.at(index).loc;
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
	if (l.x >= 0 && l.x < 10 && l.y >= 0 && l.y < 10) {
		AddUnitTo(l, index);
		return true;
	}
	return false;
}

std::string Game::PrintUnit(Unit* unit) {
	return "Name: " + unit->name + "\n"
		+ std::to_string(unit->cur.hp) + "/" + std::to_string(unit->max.hp) + " HP"
		+ "      Strength: " + std::to_string(unit->cur.strength) + "\n"
		+ std::to_string(unit->cur.mp) + "/" + std::to_string(unit->max.mp) + " MP"
		+ "      Magic:    " + std::to_string(unit->cur.magic) + "\n"
		+ "              Defense:  " + std::to_string(unit->cur.defense) + "\n"
		+ "              Agility:  " + std::to_string(unit->cur.agility);
}

std::string Game::PrintTile(Location loc) {
	Tile* tile;
	Area a = areas.at(loc.area);
	bool found = false;
	if (loc.x > 9 || loc.x < 0 || loc.y>9 || loc.y < 0)
		tile = &tiles.at(a.defaultTile);
	else {
		tile = &tiles.at(a.tiles[loc.x + loc.y * 10]);
		found = true;
	}
	std::string s = "";
	if (tile->fluff.size() > 1) {
		s = tile->fluff;
	}
	if (found && a.seenTiles[loc.x+loc.y*10]) {
		bool foundUnit = false;
		for (unsigned int i = 0; i < units.size(); i++) {
			if (i!=curPlayer && units.at(i).loc == loc) {
				if(!foundUnit)
					s += "\nYou see these characters: ";
				s += "\n" + units.at(i).name;
				foundUnit = true;
			}
		}
	}
	else if (tile->walkable) {
		s += "\nYou do not know what you will find here.";
	}
	if(!tile->walkable) {
		s += "\nYou can not go this way.";
	}
	return s;
}