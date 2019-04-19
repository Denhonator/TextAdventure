#pragma once
#include "Util.h"

class Game
{
public:
	Game();
	~Game();
	std::string ProcessCommand(std::string input, int player=0);
	void AddPlayer(std::string name, Location loc = Location{ 0,0,nullptr });
	bool AddUnitTo(Location loc, unsigned int index = 0);
	bool RemoveUnitFrom(Tile* tile, unsigned int index = 0);
	std::vector<Unit> units;
	std::vector<Area> areas;
	unsigned int curPlayer = 0;

private:
	unsigned int players = 0;
	std::string LookTile(std::string arguments = "");
	bool Move(std::string arguments = "", unsigned int index = 0);
	std::string PrintTile(Tile* tile);
};
