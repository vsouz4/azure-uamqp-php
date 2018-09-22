#include "Consumer.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/tlsio.h"
#include "azure_c_shared_utility/socketio.h"
#include "azure_uamqp_c/uamqp.h"
#include "Session.h"
#include "Message.h"

static Php::Value *callbackFn = NULL;
static bool consumerStopRunning = false;
static std::string consumerExceptionMessage = "";

static void on_link_detach_received_consumer(void* context, ERROR_HANDLE error)
{
    (void)error;
    const char* condition = NULL;
    const char* description = NULL;
    error_get_condition(error, &condition);
    error_get_description(error, &description);
    consumerExceptionMessage += "(" + std::string(condition) + ") " + std::string(description);
    consumerStopRunning = true;
}

static AMQP_VALUE on_message_received(const void* context, MESSAGE_HANDLE message)
{
    (void)context;
    Message *msg = new Message();
    msg->setMessageHandler(message);
    (*callbackFn)(Php::Object("Azure\\uAMQP\\Message", msg));

    return messaging_delivery_accepted();
}

Consumer::Consumer(Session *session, std::string resourceName)
{
    this->session = session;
    this->resourceName = resourceName;

    source = messaging_create_source(("amqps://" + session->getConnection()->getHost() + "/" + resourceName).c_str());
    target = messaging_create_target("ingress-rx");
    link = link_create(session->getSessionHandler(), "receiver-link", role_receiver, source, target);
    link_set_rcv_settle_mode(link, receiver_settle_mode_first);
    link_subscribe_on_link_detach_received(link, on_link_detach_received_consumer, session);

    amqpvalue_destroy(source);
    amqpvalue_destroy(target);

    /* create a message receiver */
    message_receiver = messagereceiver_create(link, NULL, NULL);

    if (message_receiver == NULL) {
        throw Php::Exception("Could not create message receiver");
    }

    if (session->getConnection()->isDebugOn()) {
        messagereceiver_set_trace(message_receiver, true);
    }
}

void Consumer::consume(Php::Value &callback)
{
    callbackFn = &callback;

    if (messagereceiver_open(message_receiver, on_message_received, message_receiver) != 0) {
        throw Php::Exception("Could not open the message receiver");
    }

    while (!consumerStopRunning)
    {
        session->getConnection()->doWork();
    }
    messagereceiver_destroy(message_receiver);
    link_destroy(link);
    session->close();
    session->getConnection()->close();
    if (!consumerExceptionMessage.empty()) {
        throw Php::Exception(consumerExceptionMessage);
    }
}
