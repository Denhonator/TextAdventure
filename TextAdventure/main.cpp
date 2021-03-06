#include "Game.h"
#include "NetworkTools.h"
#include <thread>

void Receiver(Server* s, Game* g, int player) {
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
			output = g->ProcessCommand(recvbuf,player);
			std::replace(output.begin(), output.end(), '_', ' ');
			std::cout << "Other player: " << recvbuf << "\n" << output << std::endl;
			s->Send(":"+output);
		}
		std::cout << "\nChoose action: ";
	}
}

int main()
{
	srand(time(NULL));
	std::string input;
	std::string output;
	Game game = Game();
	Server server = Server();

	while (true) {
		std::cout << "\nHost (h), join (j ip), quit (q) offline (any)? > ";
		std::getline(std::cin, input);
		if (input[0] == 'j') {
			if (!server.CreateClient(input.size() > 2 ? input.substr(2) : "127.0.0.1"))
				break;
		}
		else if (input[0] == 'h') {
			if (!server.CreateServer()) {
				game.AddPlayer("Human","player2");
				break;
			}
		}
		else if (input[0] == 'q')
			return 0;
		else
			break;
		server = Server();
	}
	std::thread receiver = std::thread(&Receiver, &server, &game, 1);
	while (true)
	{
		std::cout << "Choose action: ";
		std::getline(std::cin, input);
		if (input == "") {
			continue;
		}
		if (input == "q")
			break;
		else if (server.isConnected() && (input[0] == '>' || input[0] == ':' || server.isConnected()=='c'))
			server.Send(input);
		else {
			output = game.ProcessCommand(input);
			std::replace(output.begin(), output.end(), '_', ' ');
			std::cout << output << std::endl << std::endl;
			if (server.isConnected() == 's')
				server.Send(":Other player: "+input+"\n"+output);
		}
	}
	server.Shutdown();
	receiver.join();
	return 0;
}