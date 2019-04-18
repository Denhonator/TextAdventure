#pragma once
#include "Util.h"

class Game
{
public:
	Game();
	~Game();
	std::string ProcessCommand(std::string input);
	std::vector<Unit> players;
	std::vector<Area> areas;
	unsigned int curPlayer = 0;
};
