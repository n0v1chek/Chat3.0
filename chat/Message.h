#pragma once
#include <string>

class Message 
{
public:
    Message();
    Message(const std::string& name, const std::string& message);

    const std::string& getName() const;
    const std::string& getMessage() const;

private:
    std::pair<std::string, std::string> _message;

};