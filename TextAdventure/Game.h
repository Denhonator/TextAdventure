#pragma once
#include "Util.h"
#include "Resources.h"

class Game
{
public:
	Game();
	~Game();
	std::string ProcessCommand(std::string input, int player=0);
	void AddUnit(unsigned int index, char type, Location loc = Location{ 0,0,0 }, std::string name = "unit");
	bool AddUnitTo(Location loc, unsigned int index = 0);
	std::vector<Unit> units;
	std::vector<Area> areas;
	unsigned int curPlayer = 0;

private:
	unsigned int players = 0;
	std::string LookTile(std::string arguments = "");
	bool Move(std::string arguments = "", unsigned int index = 0);
	std::string PrintStats(Stats stats, char type);
	std::string PrintItem(Item* item, char type = 0);
	std::string PrintTile(Location loc);
	std::string PrintUnit(std::string arguments = "", int index = -1);
};
