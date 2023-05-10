#include "Message.h"

Message::Message()
	: _message()
{
}

Message::Message(const std::string& name, const std::string& message)
	: _message(name, message)
{

}

const std::string& Message::getName() const
{
	return _message.first;
}

const std::string& Message::getMessage() const
{
	return _message.second;
}
