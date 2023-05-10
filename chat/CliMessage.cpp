#include "CliMessage.h"

CLIMessage::CLIMessage(UserData* current)
	: _baseApp(BaseApp::instance()), _current(current)
{
}

void CLIMessage::mainMenu()
{
	while (true)
	{
		std::system("cls");

		help();

		int answer;
		std::cin >> answer;

		switch (answer)
		{
		case 1:
			sendToSomebody();

			break;
		case 2:
			mutualChat();

			break;
		case 3:
			sendToALl();

			break;
		case 4:
			generalChat();

			break;
		case 5:

			return;
		case 0:

			exit(0);
		default:
			break;
		}
	}
	
}

void CLIMessage::sendToSomebody() const
{
	std::string receiver;
	std::string message;

	CLIUserData cliUserData;
	
	while (true)
	{
		std::system("cls");

		std::cout << "Send a messege to: ";
		std::cin >> receiver;

		if (!_baseApp->isLogin(receiver))
		{
			std::system("cls");
			std::cout << receiver << " have never been registered!\n";

			if (!cliUserData.isContinue())
				return;

			continue;
		}

		if (receiver == _current->getLogin())
		{
			std::system("cls");
			std::cout << "You can't send a message to yourself!\n";

			if (!cliUserData.isContinue())
				return;

			continue;
		}

		std::cout << "Your messege to " << receiver << ": ";
		std::cin.ignore();
		std::getline(std::cin, message);


		break;
	}

	_baseApp->sendMessage(Message(_current->getLogin(), message), receiver);
}

void CLIMessage::mutualChat() const
{
	std::string chatName; 

	CLIUserData cliUserData;

	while (true)
	{
		std::system("cls");

		std::cout << "Enter chat's name: ";
		std::cin >> chatName;

		if (!_baseApp->isLogin(chatName))
		{
			std::system("cls");
			std::cout << chatName << " have never been registered!\n";

			if (!cliUserData.isContinue())
				return;

			continue;
		}

		if (chatName == _current->getLogin())
		{
			std::system("cls");
			std::cout << "You can't send a message to yourself!\n";

			if (!cliUserData.isContinue())
				return;

			continue;
		}

		break;
	}

	_baseApp->printChat(chatName);
}

void CLIMessage::sendToALl() const
{
	std::system("cls");

	std::cout << "Your messange in general chat: ";
	std::string message;

	std::cin.ignore();
	std::getline(std::cin, message);

	_baseApp->sendMessage(Message(_current->getLogin(), message));
}

void CLIMessage::generalChat() const
{
	_baseApp->printChat();
}

void CLIMessage::help()
{
	std::cout << "Your login: " << _current->getLogin() << "\n";
	std::cout << "1. Send a message to user\n";
	std::cout << "2. Look at mutual chat\n";
	std::cout << "3. Send a message to all\n";
	std::cout << "4. Look at general chat\n";
	std::cout << "5. Back\n";
	std::cout << "0. Exit\n";
}