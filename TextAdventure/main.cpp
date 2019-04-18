#include "Game.h"

int main()
{
	std::string input;
	std::string output;
	Game game = Game();
	while (true)
	{
		std::cout << "\nChoose action: ";
		std::getline(std::cin, input);
		std::cout << std::endl;
		if (input == "q")
			break;
		else {
			output = game.ProcessCommand(input);
			std::cout << output << std::endl;
		}
	}
}