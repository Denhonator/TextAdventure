#include "Game.h"

Game::Game()
{
	Resources::LoadItems();
	Resources::LoadUnits();
	AddPlayer("Human","player1");
}

Game::~Game()
{
}

std::string Game::ProcessCommand(std::string input, int player)
{
	curPlayer = player;
	if (input == "items")
		return PrintUnit(units.at(curPlayer), input);
	if (input == "stats")
		return PrintUnit(units.at(curPlayer), input);
	if (input == "move")
		return Encounter(input);
	return "Invalid command";
}

void Game::AddPlayer(std::string name, std::string newname)
{
	Unit u = Resources::GetUnit(name);
	if (newname != "")
		u.name = newname;
	units.insert(units.begin() + players, u);
	players++;
}

std::string Game::Encounter(std::string arguments)
{
	if (arguments == "move") {
		if (currentEnc.objective != "enemy" || currentEnc.stats.hp <= 0) {
			progress++;
			currentEnc = Resources::GetEncounter();
			return currentEnc.intro;
		}
		else
			return "Cannot move on, you are in combat!";
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