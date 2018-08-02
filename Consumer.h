#ifndef UAMQP_PHP_CONSUMER_H
#define UAMQP_PHP_CONSUMER_H
#include <phpcpp.h>
#include "Session.h"

class Consumer : public Php::Base
{
private:
    Session *session;
    std::string resourceName;

    LINK_HANDLE link;
    MESSAGE_RECEIVER_HANDLE message_receiver;
    AMQP_VALUE source;
    AMQP_VALUE target;

public:
    Consumer() = default;
    virtual ~Consumer() = default;

    void __construct(Php::Parameters &params);

    void consume(Php::Parameters &params);
};

#endif
