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
	Encounter currentEnc;
	std::vector<Unit> units;
	std::string Encounter(std::string arg1, std::string arg2);
	std::string GetItem(std::string arguments = "");
	std::string PrintStats(Stats stats, char type);
	std::string PrintItem(std::string name, bool detail);
	std::string PrintUnit(Unit u, std::string arguments = "");
};
