#ifndef UAMQP_PHP_CONSUMER_H
#define UAMQP_PHP_CONSUMER_H
#include <phpcpp.h>
#include "Session.h"
#include "Message.h"

class Consumer
{
private:
    Session *session;
    std::string resourceName;
    bool closeRequested = false;

    LINK_HANDLE link;
    MESSAGE_RECEIVER_HANDLE message_receiver;
    AMQP_VALUE source;
    AMQP_VALUE target;

public:
    Consumer(Session *session, std::string resourceName);
    virtual ~Consumer() = default;

    void setCallback(Php::Value &callback, Php::Value &loopFn);
    void consume();
    void close();
    bool wasCloseRequested();
};

#endif
