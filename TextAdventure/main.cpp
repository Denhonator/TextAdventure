#include "Game.h"
#include "NetworkTools.h"
#include <thread>

void Receiver(Server* s) {
	std::string recvbuf;
	while (s->isConnected()) {
		recvbuf = s->Receive();
		std::cout << "\rReceived: " << recvbuf << std::endl << "Choose action: ";
	}
}

int main()
{
	std::string input;
	std::string output;
	Game game = Game();
	Server server = Server();

	std::cout << "\nHost (h) or join (j)? > ";
	std::getline(std::cin, input);
	std::cout << std::endl;
	if (input[0] == 'j')
		server.CreateClient();
	else if (input[0]=='h')
		server.CreateServer();

	std::thread receiver = std::thread(&Receiver, &server);

	while (true)
	{
		std::cout << "\nChoose action: ";
		std::getline(std::cin, input);
		std::cout << std::endl;
		if (input == "q")
			break;
		else if (input[0] == '>')
			server.Send(input);
		else {
			output = game.ProcessCommand(input);
			std::cout << output << std::endl;
		}
	}
	server.Shutdown();
	receiver.join();
	return 0;
}