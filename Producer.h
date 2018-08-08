#ifndef UAMQP_PHP_PRODUCER_H
#define UAMQP_PHP_PRODUCER_H
#include <phpcpp.h>
#include "Session.h"

class Producer : public Php::Base
{
private:
    Session *session;
    std::string resourceName;

    LINK_HANDLE link;
    MESSAGE_SENDER_HANDLE message_sender;
    AMQP_VALUE source;
    AMQP_VALUE target;

public:
    Producer() = default;
    virtual ~Producer() = default;

    void __construct(Php::Parameters &params);

    void publish(Php::Parameters &params);
};

#endif
