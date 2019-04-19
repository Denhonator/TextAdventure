#include "Game.h"
#include "NetworkTools.h"
#include <thread>

void Receiver(Server* s, Game* g) {
	std::string recvbuf;
	std::string output;
	while (s->isConnected()) {
		recvbuf = s->Receive();
		std::cout << "\r                              \r";
		if (recvbuf[0] == '>')
			std::cout << "Message: " << recvbuf.substr(1) << std::endl;
		else if (recvbuf[0] == ':')
			std::cout << recvbuf.substr(1) << std::endl;
		else if (s->isConnected() == 's') {
			output = g->ProcessCommand(recvbuf);
			std::cout << "Other player: " << recvbuf << "\n" << output << std::endl;
			s->Send(":"+output);
		}
		std::cout << "Choose action: ";
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

	std::thread receiver = std::thread(&Receiver, &server, &game);

	while (true)
	{
		std::cout << "Choose action: ";
		std::getline(std::cin, input);
		//std::cout << std::endl;
		if (input == "q")
			break;
		else if (server.isConnected() && (input[0] == '>' || server.isConnected()=='c'))
			server.Send(input);
		else {
			output = game.ProcessCommand(input);
			std::cout << output << std::endl;
			if (server.isConnected() == 's')
				server.Send(":Other player: "+input+"\n"+output);
		}
	}
	server.Shutdown();
	receiver.join();
	return 0;
}