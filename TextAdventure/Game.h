#pragma once
#include "Util.h"
#include "Resources.h"

class Game
{
public:
	Game();
	~Game();
	std::string ProcessCommand(std::string input, int player=0);
	void AddUnit(std::string name, char type, Location loc = Location{ 0,0,0 }, std::string newname = "");
	bool AddUnitTo(Location loc, unsigned int index = 0);
	std::vector<Unit> units;
	std::vector<Area> areas;
	unsigned int curPlayer = 0;

private:
	unsigned int players = 0;
	std::string LookTile(std::string arguments = "");
	bool Move(std::string arguments = "");
	std::string GetItem(std::string arguments = "");
	std::string GetEquipment(std::string arguments = "");
	std::string PrintStats(Stats stats, char type);
	std::string PrintItem(Item* item, char type = 0);
	Tile* GetTile(Location loc);
	std::string PrintTile(Location loc);
	std::string PrintUnit(std::string arguments = "");
};
