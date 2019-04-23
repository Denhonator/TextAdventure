#include "Game.h"

Game::Game()
{
	Resources::LoadTiles();
	Resources::LoadAreas();
	Resources::LoadItems();
	Resources::LoadUnits();
	areas = Resources::areas;
	AddUnit(0, 'p', Location{ 0,0,0 },"player1");
	AddUnit(0, 'e', Location{ 1,0,0 },"enemy");
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
	if (input == "items") {
		input = "player";
		arg = "items " + arg;
	}
	if (input == "equipment") {
		input = "player";
		arg = "equipment " + arg;
	}
	if (input == "player")
		return PrintUnit(arg);
	if (input == "look")
		return LookTile(arg);
	if (input == "move") {
		if(Move(arg))
			return LookTile();
		return LookTile(arg);
	}
	if (input == "item")
		return GetItem(arg);
	if (input == "equip")
		return GetEquipment(arg);
	return "Invalid command";
}

void Game::AddUnit(unsigned int index, char type, Location loc, std::string name)
{
	Unit u = Resources::units.at(index);
	u.type = type;
	if (name != "")
		u.name = name;
	if (u.type == 'p') {
		units.insert(units.begin() + players, u);
		AddUnitTo(loc, players);
		players++;
	}
	else {
		units.push_back(u);
		AddUnitTo(loc, units.size() - 1);
	}
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

bool Game::Move(std::string arguments)
{
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
	if (l.x >= 0 && l.x < 10 && l.y >= 0 && l.y < 10) {
		AddUnitTo(l, curPlayer);
		return true;
	}
	return false;
}

std::string Game::GetItem(std::string arguments)
{
	std::string name = "", rarity = "", type = "";
	int slot = -1, count = -1;
	std::stringstream ss;
	ss << arguments;
	ss >> name >> count >> slot >> rarity >> type;
	slot--;
	if (count < 0 || slot < 0)
		return "Arguments: name count slot (1-6) (+rarity type)";
	Item* item = Resources::GetItem(name, rarity, type);
	if (item != nullptr) {
		if (item->name == units.at(curPlayer).inventory[slot].second)
			units.at(curPlayer).inventory[slot].first += count;
		else {
			units.at(curPlayer).inventory[slot].first = count;
			units.at(curPlayer).inventory[slot].second = item->name;
		}
		return PrintUnit("items");
	}
	if (name == "remove") {
		count = std::min(count, (int)units.at(curPlayer).inventory[slot].first);
		units.at(curPlayer).inventory[slot].first -= count;
		return PrintUnit("items");
	}
	return "Item not found";
}

std::string Game::GetEquipment(std::string arguments)
{
	std::string name = "", rarity = "", type = "";
	int slot = -1;
	std::stringstream ss;
	ss << arguments;
	ss >> name >> slot >> rarity >> type;
	slot--;
	if (slot < 0)
		return "Arguments: name slot (1-6) (+rarity type)";
	Item * item = Resources::GetItem(name, rarity, type);
	if (item != nullptr) {
		units.at(curPlayer).equipment[slot] = item->name;
		return PrintUnit("equipment");
	}
	if (name == "remove") {
		units.at(curPlayer).equipment[slot] = "";
		return PrintUnit("equipment");
	}
	return "Item not found";
}

std::string Game::PrintStats(Stats stats, char type)
{
	std::string hp = std::to_string(stats.hp) + " HP";
	for (unsigned int i = hp.size(); i < 5; i++)
		hp += ' ';
	std::string mp = std::to_string(stats.mp) + " MP";
	for (unsigned int i = mp.size(); i < 5; i++)
		mp += ' ';
	if (type == 'p') {
		return hp + "    Strength: " + std::to_string(stats.strength) + "\n"
			+ mp + "    Magic:    " + std::to_string(stats.magic) + "\n"
			+ "         Defense:  " + std::to_string(stats.defense) + "\n"
			+ "         Agility:  " + std::to_string(stats.agility);
	}
	if (type == 'e') {
		return "Strength: " + std::to_string(stats.strength) + "\n"
			 + "Magic:    " + std::to_string(stats.magic) + "\n"
			 + "Defense:  " + std::to_string(stats.defense) + "\n"
			 + "Agility:  " + std::to_string(stats.agility);
	}
	if (type == 'u') {
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

std::string Game::PrintUnit(std::string arguments) {
	std::string arg2="";
	std::stringstream ss;
	ss << arguments;
	ss >> arguments >> arg2;
	bool detail = arg2 != "";
	Unit* unit = &units.at(curPlayer);
	if (arguments == "stats") {
		return "Name: " + unit->name + "\n" + PrintStats(unit->stats, 'p');
	}
	std::string buf = "";
	if (arguments == "items") {
		for (unsigned int i = 0; i < sizeof(unit->inventory)/sizeof(unit->inventory[0]); i++) {
			if (unit->inventory[i].first > 0) {
				Item* item = Resources::GetItem(unit->inventory[i].second, "", "use", false);
				if (item == nullptr)
					buf += "\n(" + std::to_string(unit->inventory[i].first) + ") " + unit->inventory[i].second + " (Not found)";
				else if (!detail || item->name == arg2) {
					buf += "\n(" + std::to_string(unit->inventory[i].first) + ") " + PrintItem(item, detail ? item->type[0] : 0);
					if (detail)
						break;
				}
			}
			else if(!detail)
				buf += "\nEmpty slot";
		}
		return buf.substr(1);	//Remove first linebreak
	}
	if (arguments == "equipment") {
		for (unsigned int i = 0; i < sizeof(unit->equipment)/sizeof(unit->equipment[0]); i++) {
			Item* item = Resources::GetItem(unit->equipment[i], "", "equipment", false);
			if (item == nullptr) {
				if(unit->equipment[i].size()>1)
					buf += "\n"+unit->equipment[i] + " (Not found)";
				else if(!detail)
					buf += "\nEmpty slot";
			}
			else if (!detail || item->name == arg2) {
				buf += "\n"+PrintItem(item, detail ? item->type[0] : 0);
				if (detail)
					break;
			}
		}
		return buf.substr(1);	//Remove first linebreak
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