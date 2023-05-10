#include <limits>
#include "CliUserData.h"

CLIUserData::CLIUserData()
	: _baseApp(BaseApp::instance()), _current(nullptr)
{
}

CLIUserData::CLIUserData(UserData* current)
	: _baseApp(BaseApp::instance()), _current(current)
{
	
}

bool CLIUserData::isContinue()
{
	std::cout << "\n\nDo you wanna return back?(y/n): ";

	char answer;
	std::cin >> answer;

	std::cin.clear();
	std::cin.ignore(32767, '\n');

	if (answer == 'n' || answer == 'N')
		return false;

	return true;
}

void CLIUserData::setCurrent(UserData* current)
{
	_current = current;
}

void CLIUserData::help()
{
	std::cout << "Your login: " << _current->getLogin() << "\n";
	std::cout << "1. Go message\n";
	std::cout << "2. Settings\n";
	std::cout << "3. Back\n";
	std::cout << "0. Exit\n";
}
