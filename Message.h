#ifndef UAMQP_PHP_MESSAGE_H
#define UAMQP_PHP_MESSAGE_H
#include <phpcpp.h>

class Message : public Php::Base
{
private:
    std::string body;

public:
    Message() = default;
    virtual ~Message() = default;

    void __construct(Php::Parameters &params);

    Php::Value getBody();
    void setBody(std::string body);
};

#endif
