#pragma once
#include "Util.h"
#include "Resources.h"

class Game
{
public:
	Game();
	~Game();
	std::string ProcessCommand(std::string input, int player=0);
	void AddPlayer(std::string name, std::string newname = "");

private:
	unsigned int players = 0;
	unsigned int curPlayer = 0;
	unsigned int progress = 0;
	Tile currentTile;
	std::vector<Unit> units;
	std::string GetItem(std::string arguments = "");
	std::string GetEquipment(std::string arguments = "");
	std::string PrintStats(Stats stats, char type);
	std::string PrintItem(Item* item, char type = 0);
	std::string PrintUnit(Unit u, std::string arguments = "");
	std::string PrintTile(std::string arguments = "");
};
