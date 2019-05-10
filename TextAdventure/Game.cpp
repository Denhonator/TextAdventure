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
	if (input == "stats") {
		input = "player";
		arg = "stats";
	}
	if (input == "player")
		return PrintUnit(units.at(curPlayer), arg);
	if (input == "item")
		return GetItem(arg);
	if (input == "look")
		return PrintTile(arg);
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
	std::string name = "";
	int count = -1;
	std::stringstream ss;
	ss << arguments;
	ss >> name >> count;
	if (count < 0)
		return "Arguments: name count";
	units.at(curPlayer).inventory[name] = count;
	return "Item(s) added";
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

std::string Game::PrintItem(std::string name, bool detail)
{
	Item* item = Resources::GetItem(name,"","",false);
	if (item == nullptr)
		return name + " (not defined)";
	if (item->fluff.size()>1)
		name += " - " + item->fluff;
	return name;
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
		for (std::map<std::string, unsigned int>::iterator iter = u.inventory.begin(); iter != u.inventory.end();++iter) {
			buf += "\n(" + std::to_string(iter->second) + ") " + PrintItem(iter->first, detail);
		}
		return buf.substr(1);	//Remove first linebreak
	}
	return "Arguments: stats, items (+name for details)";
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
		buf += "\n" + currentTile.units.at(i).name;
	}
	return currentTile.fluff + buf;
}
