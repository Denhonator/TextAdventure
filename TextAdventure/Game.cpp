#include "Game.h"

Game::Game()
{
	Resources::LoadTiles();
	Resources::LoadAreas();
	Resources::LoadItems();
	Resources::LoadUnits();
	areas = Resources::areas;
	AddPlayer(0);
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
		return PrintUnit(arg);
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

void Game::AddPlayer(unsigned int index, std::string name, Location loc)
{
	Unit u = Resources::units.at(0);
	if (name != "")
		u.name = name;
	else
		u.name += std::to_string(players);
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

std::string Game::PrintStats(Stats stats, char type)
{
	std::string hp = std::to_string(stats.hp) + " HP";
	for (unsigned int i = hp.size(); i < 5; i++)
		hp += ' ';
	std::string mp = std::to_string(stats.mp) + " MP";
	for (unsigned int i = mp.size(); i < 5; i++)
		mp += ' ';
	if (type == 'u') {
		return hp + "    Strength: " + std::to_string(stats.strength) + "\n"
			+ mp + "    Magic:    " + std::to_string(stats.magic) + "\n"
			+ "         Defense:  " + std::to_string(stats.defense) + "\n"
			+ "         Agility:  " + std::to_string(stats.agility);
	}
	if (type == 'e') {
		return "    Strength: " + std::to_string(stats.strength) + "\n"
			 + "    Magic:    " + std::to_string(stats.magic) + "\n"
			 + "    Defense:  " + std::to_string(stats.defense) + "\n"
			 + "    Agility:  " + std::to_string(stats.agility);
	}
	if (type == 'i') {
		return hp + " " + mp;
	}
}

std::string Game::PrintItem(Item* item, char type)
{
	std::string buf = "";
	buf += item->name;
	if (item->fluff != "-")
		buf += " - " + item->fluff;
	if (type)
		buf += "\n" + PrintStats(item->stats, type);
	return buf;
}

std::string Game::PrintUnit(std::string arguments, int index) {
	std::string arg2="";
	std::stringstream ss;
	ss << arguments;
	ss >> arguments >> arg2;
	char detail = arg2 != "";
	index = index >= 0 ? index : curPlayer;
	Unit* unit = &units.at(index);
	if (arguments == "stats") {
		return "Name: " + unit->name + "\n" + PrintStats(unit->stats, 'u');
	}
	std::string buf = "";
	if (arguments == "items") {
		detail = detail ? 'i' : 0;
		for (unsigned int i = 0; i < sizeof(unit->inventory)/sizeof(unit->inventory[0]); i++) {
			if (i > 0 && (unit->inventory[i].first > 0 || !detail))
				buf += "\n";
			if (unit->inventory[i].first > 0) {
				Item* item = &Resources::items.at(unit->inventory[i].second);
				if (!detail || item->name == arg2)
					buf += '('+std::to_string(unit->inventory[i].first) + ") " + PrintItem(item, detail);
			}
			else if(!detail)
				buf += "Empty slot";
		}
		return buf;
	}
	if (arguments == "equipment") {
		detail = detail ? 'e' : 0;
		for (unsigned int i = 0; i < sizeof(unit->equipment)/sizeof(unit->equipment[0]); i++) {
			if (i > 0 && (unit->equipment[i] >= 0 || !detail))
				buf += "\n";
			if (unit->equipment[i] >= 0) {
				Item* item = &Resources::items.at(unit->equipment[i]);
				if (!detail || item->name == arg2)
					buf += PrintItem(item, detail);
			}
			else if(!detail)
				buf += "Empty slot";
		}
		return buf;
	}
	return "Arguments: stats, items, equipment (+name for details)";
}

std::string Game::PrintTile(Location loc) {
	Tile* tile;
	Area a = areas.at(loc.area);
	bool found = false;
	if (loc.x > 9 || loc.x < 0 || loc.y>9 || loc.y < 0)
		tile = &Resources::tiles.at(a.defaultTile);
	else {
		tile = &Resources::tiles.at(a.tiles[loc.x + loc.y * 10]);
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