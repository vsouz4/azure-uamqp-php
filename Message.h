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
    AMQP_VALUE application_properties_map;
    AMQP_VALUE annotations_map;
    BINARY_DATA binary_data;

public:
    Message() = default;
    virtual ~Message() = default;

    void __construct(Php::Parameters &params);

    void setMessageHandler(MESSAGE_HANDLE message);
    MESSAGE_HANDLE getMessageHandler();
    Php::Value getBody();
    void setBody(std::string body);
    Php::Value getApplicationProperty(Php::Parameters &params);
    void setApplicationProperty(Php::Parameters &params);
    void setMessageAnnotation(Php::Parameters &params);
};

#endif
