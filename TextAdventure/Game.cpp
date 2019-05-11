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
	std::stringstream ss;
	std::string arg;
	ss << input;
	ss >> input >> arg;
	curPlayer = player;
	if (input == "items")
		return PrintUnit(units.at(curPlayer), input);
	if (input == "stats")
		return PrintUnit(units.at(curPlayer), input);
	if (input == "move" || input == "fight")
		return Encounter(input, arg);
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

std::string Game::Encounter(std::string arg1, std::string arg2)
{
	std::string buf = "";
	int damage, mdamage, hpStart;
	if (arg1 == "move") {
		if (currentEnc.objective != "enemy" || currentEnc.stats.hp <= 0) {
			progress++;
			currentEnc = Resources::GetEncounter();
			return currentEnc.intro;
		}
		else
			return "Cannot move on, you are in combat!";
	}
	if (arg1 == "fight") {
		if (currentEnc.objective == "enemy" && currentEnc.stats.hp > 0) {
			Item* it = nullptr;
			if (units.at(curPlayer).inventory.find(arg2) != units.at(curPlayer).inventory.end())
				it = Resources::GetItem(arg2, "", "", false);
			if (it == nullptr)
				return "Invalid second argument. Should be an item you own.";
			if (rand() % 20 > 9 - units.at(curPlayer).stats.agility + currentEnc.stats.agility) {
				damage = std::max(0, units.at(curPlayer).stats.strength - currentEnc.stats.defense);
				mdamage = std::max(0, units.at(curPlayer).stats.magic - currentEnc.stats.defense);
				hpStart = currentEnc.stats.hp;
				currentEnc.stats.hp -= it->stats.physical * damage * currentEnc.resist.physical;
				currentEnc.stats.hp -= it->stats.fire * mdamage * currentEnc.resist.fire;
				currentEnc.stats.hp -= it->stats.ice * mdamage * currentEnc.resist.ice;
				currentEnc.stats.hp -= it->stats.lightning * mdamage * currentEnc.resist.lightning;
				currentEnc.stats.hp -= it->stats.neutral * mdamage * currentEnc.resist.neutral;
				buf += "Hit! Dealt " + std::to_string(hpStart - currentEnc.stats.hp) + " damage!";
			}
			else
				buf += "Missed!";
			if (currentEnc.stats.hp > 0) {
				if (rand() % 20 > 9 + units.at(curPlayer).stats.agility - currentEnc.stats.agility) {
					damage = std::max(0, currentEnc.stats.strength - units.at(curPlayer).stats.defense);
					mdamage = std::max(0, currentEnc.stats.magic - units.at(curPlayer).stats.defense);
					hpStart = units.at(curPlayer).stats.hp;
					units.at(curPlayer).stats.hp -= currentEnc.attack.physical * damage;
					units.at(curPlayer).stats.hp -= currentEnc.attack.fire * mdamage;
					units.at(curPlayer).stats.hp -= currentEnc.attack.ice * mdamage;
					units.at(curPlayer).stats.hp -= currentEnc.attack.lightning * mdamage;
					units.at(curPlayer).stats.hp -= currentEnc.attack.neutral * mdamage;
					buf += "\nEnemy hit! Dealt " + std::to_string(hpStart - units.at(curPlayer).stats.hp) + " damage!";
				}
				else
					buf += "\nEnemy missed!";
			}
			else
				buf += "\nEnemy has been defeated!";
		}
		else
			buf += "No enemy to fight";
		if (units.at(curPlayer).stats.hp <= 0)
			buf += "\nYou have died.";
		return buf;
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
			if(!detail||iter->first==arg2)
				buf += "\n(" + std::to_string(iter->second) + ") " + PrintItem(iter->first, detail);
		}
		return buf.substr(1);	//Remove first linebreak
	}
	return "Arguments: stats, items (+name for details)";
}