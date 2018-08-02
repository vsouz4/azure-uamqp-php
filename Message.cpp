#include "Message.h"

void Message::__construct(Php::Parameters &params)
{
    body = params[0].stringValue();
}

Php::Value Message::getBody()
{
    return body;
}

void Message::setBody(std::string body)
{
    this->body = body;
}
