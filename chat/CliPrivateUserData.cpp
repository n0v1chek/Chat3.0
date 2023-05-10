#include "CliPrivateUserData.h"

CLIPrivateUserData::CLIPrivateUserData()
    : _baseApp(BaseApp::instance())
{
}

std::pair<std::string, std::string> CLIPrivateUserData::getLoginAndPass()
{
    std::string login, password;

    std::system("cls");

    std::cout << "Login: ";
    std::cin >> login;

    std::cout << "Password: ";
    std::cin >> password;

    return { login,password };
}

void CLIPrivateUserData::help()
{
}
