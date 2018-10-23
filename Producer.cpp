#include "Producer.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/tlsio.h"
#include "azure_c_shared_utility/socketio.h"
#include "azure_uamqp_c/uamqp.h"
#include "Session.h"
#include "Message.h"

static unsigned int sent_messages = 0;
static bool producerStopRunning = false;
std::string producerExceptionMessage = "";

static void on_link_detach_received_producer(void* context, ERROR_HANDLE error)
{
    (void)error;
    const char* condition = NULL;
    const char* description = NULL;
    error_get_condition(error, &condition);
    error_get_description(error, &description);
    producerExceptionMessage += "(" + std::string(condition) + ") " + std::string(description);
    producerStopRunning = true;
}

static void on_message_send_complete(void* context, MESSAGE_SEND_RESULT send_result, AMQP_VALUE delivery_state)
{
    (void)send_result;
    (void)context;
    (void)delivery_state;

    sent_messages++;
}

Producer::Producer(Session *session, std::string resourceName)
{
    this->session = session;
    this->resourceName = resourceName;

    source = messaging_create_source("ingress");
    target = messaging_create_target(("amqps://" + session->getConnection()->getHost() + "/" + resourceName).c_str());
    link = link_create(session->getSessionHandler(), "sender-link", role_sender, source, target);
    link_set_snd_settle_mode(link, sender_settle_mode_unsettled);
    (void)link_set_max_message_size(link, 65536);
    link_subscribe_on_link_detach_received(link, on_link_detach_received_producer, session);

    amqpvalue_destroy(source);
    amqpvalue_destroy(target);

    /* create a message sender */
    message_sender = messagesender_create(link, NULL, NULL);

    if (message_sender == NULL) {
        throw Php::Exception("Could not create message sender");
    }

    if (session->getConnection()->isDebugOn()) {
        messagesender_set_trace(message_sender, true);
    }
}

void Producer::publish(Message *message)
{
    MESSAGE_HANDLE msg = message->getMessageHandler();
    sent_messages = 0;

    if (messagesender_open(message_sender) != 0) {
        throw Php::Exception("Error creating messaging sender");
    }

    (void)messagesender_send_async(message_sender, msg, on_message_send_complete, msg, 10000);

    while (!producerStopRunning)
    {
        session->getConnection()->doWork();

        if (sent_messages == 1)
        {
            break;
        }
    }
    message_destroy(msg);
    messagesender_destroy(message_sender);
    link_destroy(link);
    session->close();
    session->getConnection()->close();
    if (!producerExceptionMessage.empty()) {
        throw Php::Exception(producerExceptionMessage);
    }
}
