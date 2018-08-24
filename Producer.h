#ifndef UAMQP_PHP_PRODUCER_H
#define UAMQP_PHP_PRODUCER_H
#include <phpcpp.h>
#include "Session.h"
#include "Message.h"

class Producer
{
private:
    Session *session;
    std::string resourceName;

    LINK_HANDLE link;
    MESSAGE_SENDER_HANDLE message_sender;
    AMQP_VALUE source;
    AMQP_VALUE target;

public:
    Producer(Session *session, std::string resourceName);
    virtual ~Producer() = default;

    void publish(Message *message);
};

#endif
