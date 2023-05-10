#include "PrivateUserData.h"

PrivateUserData::PrivateUserData()
	: _pData(), _messages()
{
}

PrivateUserData::PrivateUserData(const std::string& login, const std::string& password)
	: _pData(login, password)
{
}

PrivateUserData::PrivateUserData(const PrivateUserData& pud)
	: _pData(pud._pData), _messages(pud._messages)
{
}

std::string PrivateUserData::getLogin() const
{
	return _pData.first;
}

std::string PrivateUserData::getPassword() const
{
	return _pData.second;
}

std::map<std::string, std::vector<Message>>& PrivateUserData::getMessages()
{
	return _messages;
}
