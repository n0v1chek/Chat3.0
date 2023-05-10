#include "CliBaseApp.h"
#include "MySocket.h"


CLIBaseApp::CLIBaseApp()
    : _baseApp(BaseApp::instance()), _current(nullptr)
{
}

CLIBaseApp::~CLIBaseApp()
{
}

void CLIBaseApp::signIn()
{
    std::string login, password;

    CLIPrivateUserData cliPrivateUserData;
    CLIUserData cliUserData;

    while (true)
    {
        std::tie(login, password) = cliPrivateUserData.getLoginAndPass();

        if (!_baseApp->verifyUserData(login,password))//if (!_baseApp->isLogin(login) && !_baseApp->isPassword(password))
        {
            std::system("cls");

            std::cout << "Incorrect input\n";

            if (!cliUserData.isContinue())
                return;

            continue;
        }


        break;
    }

    _current = _baseApp->findUser(login);

    _baseApp->setCurrent(_current);

    cliUserData.setCurrent(_current);
    CLIMessage cliMessage(_current);

    while (true)
    {
        std::system("cls");

        cliUserData.help();

        int answer;
        std::cin >> answer;

        switch (answer)
        {
        case 1:
            cliMessage.mainMenu();

            break;
        case 2:

            // settings ( just for decency )

            break;
        case 3:

            return;
        case 0:

            exit(0);
        default:
            break;
        }
    }
}


void CLIBaseApp::signUp() const
{
    std::string login, password;
    CLIPrivateUserData cliPrivateUserData;
    CLIUserData cliUserData;
    
    while (true)
    {
        std::tie(login, password) = cliPrivateUserData.getLoginAndPass();
        
        if (_baseApp->isLogin(login))
        {
            std::system("cls");

            std::cout << "User with \"" << login << "\" login doesn't exist";
            
            if (!cliUserData.isContinue())
                return;

            continue;
        }

        
        break;
    }

    _baseApp->addUser(UserData(login, password));
}

void CLIBaseApp::help()
{
    std::cout << "1. Sign in\n";
    std::cout << "2. Sign up\n";
    std::cout << "0. Exit\n";
}