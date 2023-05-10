#include "UserData.h"

UserData::UserData()
	: PrivateUserData()
{

}

UserData::UserData(const std::string& login, const std::string& password)
	: PrivateUserData(login, password)
{
}

UserData::UserData(const UserData& ud)
	: PrivateUserData(ud)
{
}
