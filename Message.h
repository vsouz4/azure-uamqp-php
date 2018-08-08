#ifndef UAMQP_PHP_MESSAGE_H
#define UAMQP_PHP_MESSAGE_H
#include <phpcpp.h>
#include "azure_uamqp_c/uamqp.h"

class Message : public Php::Base
{
private:
    std::string body;
    MESSAGE_HANDLE message;
    AMQP_VALUE application_properties;
    BINARY_DATA binary_data;

public:
    Message() = default;
    virtual ~Message() = default;

    void __construct(Php::Parameters &params);

    void setMessageHandler(MESSAGE_HANDLE message);
    MESSAGE_HANDLE getMessageHandler();
    AMQP_VALUE map;
    Php::Value getBody();
    void setBody(std::string body);
    Php::Value getApplicationProperty(Php::Parameters &params);
    void setApplicationProperty(Php::Parameters &params);
};

#endif
