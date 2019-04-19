#include "Game.h"

Area CreateArea(std::string name);
Unit CreatePlayer(std::string name);
std::string PrintUnit(Unit* unit);

Game::Game()
{
	areas.push_back(CreateArea("test"));
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
	units.insert(units.begin()+players,CreatePlayer(name));
	AddUnitTo(loc, players);
	players++;
}

bool Game::AddUnitTo(Location loc, unsigned int index)
{
	if (loc.x >= 0 && loc.x < 10 && loc.y >= 0 && loc.y < 10) {
		if (loc.area == nullptr)
			loc.area = &areas.at(0);
		loc.tile = &loc.area->tiles[loc.x + loc.y * 10];
		loc.tile->units.push_back(index);
		units.at(index).loc = loc;
		if (units.at(index).type == 'p')
			loc.tile->seen = true;
		return true;
	}
	return false;
}

bool Game::RemoveUnitFrom(Tile* tile, unsigned int index)
{
	tile->units.erase(std::find(tile->units.begin(), tile->units.end(), index));
	return true;
}

std::string Game::LookTile(std::string arguments) {
	if (arguments == "") {
		return PrintTile(units.at(curPlayer).loc.tile);
	}
	Location l = units.at(curPlayer).loc;
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
		if (RemoveUnitFrom(units.at(index).loc.tile, index))
			return AddUnitTo(l, index);
	}
	return false;
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

Unit CreatePlayer(std::string name) {
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

std::string PrintUnit(Unit* unit) {
	return "Name: " + unit->name + "\n"
		+ std::to_string(unit->cur.hp) + "/" + std::to_string(unit->max.hp) + " HP"
		+ "      Strength: " + std::to_string(unit->cur.strength) + "\n"
		+ std::to_string(unit->cur.mp) + "/" + std::to_string(unit->max.mp) + " MP"
		+ "      Magic:    " + std::to_string(unit->cur.magic) + "\n"
		+ "              Defense:  " + std::to_string(unit->cur.defense) + "\n"
		+ "              Agility:  " + std::to_string(unit->cur.agility);
}

std::string Game::PrintTile(Tile* tile) {
	if (tile == nullptr)
		return "You can not go this way";
	std::string s = "";
	if (tile->fluff.size() > 1) {
		s = tile->fluff;
	}
	if (tile->seen) {
		for (unsigned int i = 0; i < tile->units.size(); i++) {
			if (tile->units.size() == 1 && tile->units.at(0) == curPlayer)
				break;
			if (i == 0) {
				s += "\nYou see these characters: ";
			}
			if(tile->units.at(i)!=curPlayer)
				s += "\n" + units.at(tile->units.at(i)).name;
		}
		for (unsigned int i = 0; i < tile->items.size(); i++) {
			if (i == 0) {
				s += "\nYou see these items: ";
			}
			s += "\n" + tile->items.at(i).name;
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