#include "Game.h"

Game::Game()
{
	Resources::LoadItems();
	Resources::LoadUnits();
	Resources::LoadTiles();
	currentTile = Resources::tiles.at(0);
	AddPlayer("Human","player1");
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
	if (input == "stats") {
		input = "player";
		arg = "stats";
	}
	if (input == "player")
		return PrintUnit(units.at(curPlayer), arg);
	if (input == "item")
		return GetItem(arg);
	if (input == "equip")
		return GetEquipment(arg);
	if (input == "look") {
		return PrintTile(arg);
	}
	return "Invalid command";
}

void Game::AddPlayer(std::string name, std::string newname)
{
	Unit u = Resources::GetUnit(name);
	if (u.type) {
		u.type = 'p';
		if (newname != "")
			u.name = newname;
		units.insert(units.begin() + players, u);
		players++;
	}
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
		return PrintUnit(units.at(curPlayer), "items");
	}
	if (name == "remove") {
		count = std::min(count, (int)units.at(curPlayer).inventory[slot].first);
		units.at(curPlayer).inventory[slot].first -= count;
		return PrintUnit(units.at(curPlayer), "items");
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
		return PrintUnit(units.at(curPlayer), "equipment");
	}
	if (name == "remove") {
		units.at(curPlayer).equipment[slot] = "";
		return PrintUnit(units.at(curPlayer), "equipment");
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
	return "Unable to print stats of unit";
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

std::string Game::PrintUnit(Unit u, std::string arguments) {
	std::string arg2="";
	std::stringstream ss;
	ss << arguments;
	ss >> arguments >> arg2;
	bool detail = arg2 != "";
	if (arguments == "stats") {
		return "Name: " + u.name + "\n" + PrintStats(u.stats, 'p');
	}
	std::string buf = "";
	if (arguments == "items") {
		for (unsigned int i = 0; i < sizeof(u.inventory)/sizeof(u.inventory[0]); i++) {
			if (u.inventory[i].first > 0) {
				Item* item = Resources::GetItem(u.inventory[i].second, "", "use", false);
				if (item == nullptr)
					buf += "\n(" + std::to_string(u.inventory[i].first) + ") " + u.inventory[i].second + " (Not found)";
				else if (!detail || item->name == arg2) {
					buf += "\n(" + std::to_string(u.inventory[i].first) + ") " + PrintItem(item, detail ? item->type[0] : 0);
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
		for (unsigned int i = 0; i < sizeof(u.equipment)/sizeof(u.equipment[0]); i++) {
			Item* item = Resources::GetItem(u.equipment[i], "", "equipment", false);
			if (item == nullptr) {
				if(u.equipment[i].size()>1)
					buf += "\n"+u.equipment[i] + " (Not found)";
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

std::string Game::PrintTile(std::string arguments)
{
	unsigned int index = arguments != "" ? std::stoi(arguments) : 0;
	std::string buf = "";
	if (index > 0 && currentTile.units.size()>=index) {
		return PrintUnit(currentTile.units.at(index - 1),"stats");
	}
	for (unsigned int i = 0; i < currentTile.units.size(); i++) {
		if (buf == "")
			buf = "\nYou see:";
		buf += "\n(" + std::to_string(i+1) + ") " + currentTile.units.at(i).name;
	}
	return currentTile.fluff + buf;
}
