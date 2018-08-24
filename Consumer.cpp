#include "Consumer.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/tlsio.h"
#include "azure_c_shared_utility/socketio.h"
#include "azure_uamqp_c/uamqp.h"
#include "Session.h"
#include "Message.h"

void Consumer::__construct(Php::Parameters &params)
{
    session = (Session*) params[0].implementation();
    resourceName = params[1].stringValue();

    source = messaging_create_source(("amqps://" + session->getConnection()->getHost() + "/" + resourceName).c_str());
    target = messaging_create_target("ingress-rx");
    link = link_create(session->getSessionHandler(), "receiver-link", role_receiver, source, target);
    link_set_rcv_settle_mode(link, receiver_settle_mode_first);

    amqpvalue_destroy(source);
    amqpvalue_destroy(target);
}

static Php::Value callback;

static AMQP_VALUE on_message_received(const void* context, MESSAGE_HANDLE message)
{
    (void)context;

    Message *msg = new Message();
    msg->setMessageHandler(message);
    callback(Php::Object("Azure\\uAMQP\\Message", msg));

    return messaging_delivery_accepted();
}

void Consumer::consume(Php::Parameters &params)
{
    callback = params[0];

    /* create a message receiver */
    message_receiver = messagereceiver_create(link, NULL, NULL);

    if (message_receiver == NULL) {
        throw Php::Exception("Could not create message receiver");
    }

    if (session->getConnection()->isDebugOn()) {
        messagereceiver_set_trace(message_receiver, true);
    }

    if (messagereceiver_open(message_receiver, on_message_received, message_receiver) != 0) {
        throw Php::Exception("Could not open the message receiver");
    }

    while (true)
    {
        session->getConnection()->doWork();
    }

    messagereceiver_destroy(message_receiver);
    link_destroy(link);
    session->close();
    session->getConnection()->close();
}
