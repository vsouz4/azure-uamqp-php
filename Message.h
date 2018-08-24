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
    AMQP_VALUE annotations_map;
    BINARY_DATA binary_data;

public:
    Message();
    virtual ~Message() = default;

    void setMessageHandler(MESSAGE_HANDLE message);
    MESSAGE_HANDLE getMessageHandler();
    void setBody(std::string body);

    void __construct(Php::Parameters &params);
    Php::Value getBody();
    Php::Value getApplicationProperty(Php::Parameters &params);
    void setApplicationProperty(Php::Parameters &params);
    Php::Value getMessageAnnotation(Php::Parameters &params);
    void setMessageAnnotation(Php::Parameters &params);
};

#endif
